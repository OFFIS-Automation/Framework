#include "ControlFlowBlock.h"

#include <QGraphicsSceneEvent>
#include <QGraphicsScene>
#include <QPainter>

namespace Scratch
{

ControlFlowBlock::ControlFlowBlock(const std::string& name, const size_t& numberOfBodies)
	: ArgumentItem(name)
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

	Item& item = Item::unpackItem(*event);

	if (item.itemType() != Item::Type::Block)
	{
		ArgumentItem::dragMoveEvent(event);

		return;
	}

	if (!inBodyRange(position))
	{
		Block::dragMoveEvent(event);

		return;
	}

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Block& block = *dynamic_cast<Block*>(&item);

	if (isSelfOrAncestor(block))
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

	if (item->itemType() != Item::Type::Block)
	{
		ArgumentItem::dropEvent(event);

		return;
	}

	if (item->itemType() == Item::Type::Block && !inBodyRange(position))
	{
		Block::dropEvent(event);

		return;
	}

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

	auto argumentArea = QRect(0, 0, m_width, m_headerHeight);

	if (argumentArea.contains(triggerPosition))
	{
		auto actualD = ArgumentItem::resizeBy(dx, dy, triggerPosition, m_headerHeight);

		actualDx = std::max(m_width + actualD.x(), m_defaultWidth) - m_width;
		actualDy = std::max(m_headerHeight + actualD.y(), s_defaultHeaderHeight) - m_headerHeight;

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

void ControlFlowBlock::addArgument(const std::string& name, const Item::Type& type,
	const bool enable)
{
	ArgumentItem::addArgument(name, type, m_headerHeight, enable);
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
