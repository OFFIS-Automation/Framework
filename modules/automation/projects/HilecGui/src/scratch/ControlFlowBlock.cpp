#include "ControlFlowBlock.h"

#include <QGraphicsSceneEvent>
#include <QGraphicsScene>
#include <QPainter>

namespace Scratch
{

ControlFlowBlock::ControlFlowBlock(const std::string& name, const size_t& numberOfBodies)
	: ArgumentItem(name),
	  m_defaultHeight{m_height}
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

	auto handleEvent = [&]()
	{
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
	};

	auto y = m_headerHeight;

	for (auto &body : m_bodies)
	{
		if (inConnectorActivationRange(position, y))
		{
			handleEvent();

			auto& block = *dynamic_cast<Block*>(item);

			addBody(block, body.block, QPoint(s_shaftExtent, y));

			return;
		}

		y += body.height + s_defaultHeaderHeight;
	}
}

bool ControlFlowBlock::updateItem()
{
	prepareGeometryChange();

	const auto oldWidth = m_width;

	ArgumentItem::updateItem();

	m_headerHeight = m_height;
	size_t position = m_headerHeight;

	for (auto& body : m_bodies)
	{
		if (!body.block)
		{
			body.height = defaultBodyHeight();
			position += body.height + s_shaftExtent;

			continue;
		}

		const auto oldPosition = body.block->pos();

		body.block->setPos(s_shaftExtent, position);

		if (body.block->pos() != oldPosition)
			body.block->updateItem();

		body.height = 0;

		for (Block* currentBlock = body.block; currentBlock;
				currentBlock = currentBlock->m_successor)
			body.height += currentBlock->m_height;

		position += body.height + s_shaftExtent;
	}

	const auto newHeight = position;

	if (m_width == oldWidth && newHeight == m_height)
		return false;

	m_height = newHeight;

	Block::updateItem();

	if (m_parent)
		m_parent->updateItem();

	return true;
}

void ControlFlowBlock::addArgument(const std::string& name, const Item::Type& type,
	const bool enable)
{
	ArgumentItem::addArgument(name, type, enable);
}

void ControlFlowBlock::addBody(Block& block, Block*& bodyBlock, const QPoint &offset)
{
	const auto dy = block.m_height - !bodyBlock * defaultBodyHeight();

	block.setPredecessorsReference(&bodyBlock);
	block.setSuccessor(bodyBlock);
	block.setParent(this);
	block.setPos(offset);

	bodyBlock = &block;

	updateItem();
}

} // namespace Scratch
