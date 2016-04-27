#include "ControlFlowBlock.h"

#include <QGraphicsSceneEvent>
#include <QGraphicsScene>
#include <QPainter>

namespace Scratch
{

ControlFlowBlock::ControlFlowBlock(const int width, const int height,
		const size_t& numberOfBodies)
	: Block(width, height)
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

void ControlFlowBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos().toPoint();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() == Item::Type::Condition)
	{
		if (!conditionArea().contains(position))
			return;
	}
	else if (item.itemType() == Item::Type::Block)
	{
		Block& block = *reinterpret_cast<Block*>(&item);

		if (isSelfOrAncestor(block))
			return;

		auto y = m_headerHeight;

		const auto inBodyRange = m_bodies.cend()
			!= std::find_if(m_bodies.cbegin(), m_bodies.cend(), [&](const auto& body)
				{
					const auto& isInRange =
						inConnectorActivationRange(position, y);

					y += body.height + s_defaultHeaderHeight;

					return isInRange;
				});

		if (!inConnectorActivationRange(position, 0)
				&& !inConnectorActivationRange(position, m_height)
				&& !inBodyRange)
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
	const auto& position = event->pos();

	const auto oldHeight = m_height;
	const auto oldHeaderHeight = m_headerHeight;
	const auto oldBodies = m_bodies;

	auto* item = &Item::unpackItem(*event);

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
		auto& condition = *reinterpret_cast<Condition*>(item);

		addCondition(condition);
	}
	// Block
	else
	{
		auto& block = *reinterpret_cast<Block*>(item);

		if (inConnectorActivationRange(position, 0))
			addAbove(block);
		else if (inConnectorActivationRange(position, oldHeight))
			addBelow(block);
		else
		{
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
}

QRect ControlFlowBlock::conditionArea()
{
	return QRect(s_shaftExtent + s_margin, s_margin,
		m_width - s_shaftExtent - 2 * s_margin, m_headerHeight - 2 * s_margin);
}

void ControlFlowBlock::resizeBy(int dx, int dy, const QPoint& triggerPosition)
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
			return;

		m_headerHeight += actualDy;
		update(boundingRect());
		moveBodies(m_bodies.begin(), actualDy);

		Block::resizeBy(actualDx, actualDy, triggerPosition);

		return;
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
			return;

		body->height += actualDy;
		moveBodies(body + 1, actualDy);

		break;
	}

	Block::resizeBy(actualDx, actualDy, triggerPosition);
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
