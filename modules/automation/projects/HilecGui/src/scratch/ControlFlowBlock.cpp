#include "ControlFlowBlock.h"

#include <QGraphicsSceneEvent>
#include <QGraphicsScene>
#include <QPainter>

namespace Scratch
{

ControlFlowBlock::ControlFlowBlock(const size_t& numberOfBodies)
{
	m_bodies.resize(numberOfBodies);

	for (auto &body : m_bodies)
		body.height = defaultBodyHeight();
}

void ControlFlowBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*,
	QWidget*)
{
	QPolygon polygon;
	drawOutline(polygon);

	auto y = m_headerHeight;

	std::for_each(m_bodies.cbegin(), m_bodies.cend(), [&](const auto& body)
		{
			polygon << QPoint(m_width, y);
			drawConnector(polygon, s_shaftExtent, y, true);
			polygon << QPoint(s_shaftExtent, y);

			y += body.height;

			polygon << QPoint(s_shaftExtent, y);
			drawConnector(polygon, s_shaftExtent, y);
			polygon << QPoint(m_width, y);

			y += s_defaultHeaderHeight;
		});

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

	polygon.clear();

	Condition::drawOutline(polygon, conditionArea());

	painter->setBrush(m_outlineStyle.color());
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(polygon);
}

bool ControlFlowBlock::inBodyRange(const QPoint& position)
{
	auto y = m_headerHeight;

	const auto body = std::find_if(m_bodies.cbegin(), m_bodies.cend(), [&](const auto& body)
			{
				const auto& isInRange =
					inConnectorActivationRange(position, y);

				y += body.height + s_defaultHeaderHeight;

				return isInRange;
			});

	return body != m_bodies.cend();
}

void ControlFlowBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos().toPoint();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Block::dragMoveEvent(event);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() == Item::Type::Condition)
	{
		if (m_condition || !conditionArea().contains(position))
			return;
	}
	else if (item.itemType() == Item::Type::Block)
	{
		Block& block = *dynamic_cast<Block*>(&item);

		if (isSelfOrAncestor(block))
			return;

		if (!inBodyRange(position))
			return;
	}
	else
		return;

	if (item.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);
}

void ControlFlowBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos().toPoint();

	const auto oldHeight = m_height;
	const auto oldHeaderHeight = m_headerHeight;
	const auto oldBodies = m_bodies;

	auto* item = &Item::unpackItem(*event);

	Block::dropEvent(event);

	if (item->itemType() == Item::Type::Block && !inBodyRange(position))
		return;

	event->accept();

	// Copy
	if (item->scene() != this->scene())
	{
		event->setDropAction(Qt::CopyAction);

		item = &item->clone();
	}
	// Move
	else
	{
		event->setDropAction(Qt::MoveAction);
		item->remove();
	}

	// Condition
	if (item->itemType() == Item::Type::Condition)
	{
		auto& condition = *static_cast<Condition*>(item);

		addCondition(condition);
	}
	// Block
	else
	{
		auto& block = *dynamic_cast<Block*>(item);

		auto oldY = oldHeaderHeight;
		auto oldBody = oldBodies.cbegin();

		auto y = m_headerHeight;

		for (auto &body : m_bodies)
		{
			if (inConnectorActivationRange(position, oldY))
			{
				addBody(block, body.block, QPoint(s_shaftExtent, y));

				return;
			}

			oldY += oldBody->height + s_defaultHeaderHeight;
			++oldBody;

			y += body.height + s_defaultHeaderHeight;
		}
	}
}

QRect ControlFlowBlock::conditionArea()
{
	return QRect(s_shaftExtent + s_margin, s_margin,
		m_width - s_shaftExtent - 2 * s_margin, m_headerHeight - 2 * s_margin);
}

QPoint ControlFlowBlock::resizeBy(int dx, int dy, const QPoint& triggerPosition)
{
	auto actualDx = dx;
	auto actualDy = dy;

	auto moveBodies = [&](auto&& start, auto&& dy)
	{
		std::for_each(start, m_bodies.end(), [&](auto& body)
		{
			if (body.block)
			{
				body.block->moveBy(0, dy);
				body.block->updateSuccessorPositions(0, dy);
			}
		});
	};

	if (conditionArea().contains(triggerPosition))
	{
		actualDx = std::max(m_width + dx, m_defaultWidth) - m_width;
		actualDy = std::max(m_headerHeight + dy, s_defaultHeaderHeight) - m_headerHeight;

		if (!actualDx && !actualDy)
			return QPoint();

		m_headerHeight += actualDy;
		update(boundingRect());
		moveBodies(m_bodies.begin(), actualDy);

		Block::resizeBy(actualDx, actualDy, triggerPosition);

		return QPoint(actualDx, actualDy);
	}

	auto y = m_headerHeight;

	for (auto body = m_bodies.begin(); body != m_bodies.end(); ++body)
	{
		y += body->height;

		if (triggerPosition.y() > y)
		{
			y += s_defaultHeaderHeight;

			continue;
		}

		actualDy = std::max(body->height + dy, defaultBodyHeight()) - body->height;

		if (!actualDy)
			return QPoint();

		body->height += actualDy;
		moveBodies(body + 1, actualDy);

		break;
	}

	Block::resizeBy(actualDx, actualDy, triggerPosition);

	return QPoint(actualDx, actualDy);
}

void ControlFlowBlock::addCondition(Condition& condition)
{
	const auto dWidth = condition.m_width - defaultConditionWidth();
	const auto dHeight = condition.m_height - defaultConditionHeight();

	const QPoint offset(s_shaftExtent + s_margin, s_margin);

	condition.setParent(this);
	condition.m_parentReference = reinterpret_cast<Parameter**>(&m_condition);
	condition.setPos(offset);

	m_condition = &condition;
	resizeBy(dWidth, dHeight, offset);
}

void ControlFlowBlock::addBody(Block& block, Block*& bodyBlock, const QPoint &offset)
{
	const auto dy = block.m_height - !bodyBlock * defaultBodyHeight();

	block.setPredecessorsReference(&bodyBlock);
	block.setSuccessor(bodyBlock);
	block.setParent(this);
	block.setPos(offset);

	resizeBy(0, dy, offset);

	bodyBlock = &block;
}

} // namespace Scratch
