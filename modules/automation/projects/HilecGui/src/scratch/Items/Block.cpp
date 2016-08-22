#include <stdexcept>
#include <ostream>
#include <array>

#include <QGraphicsScene>
#include <QPainter>
#include <Qt>
#include <QGraphicsSceneDragDropEvent>

#include "Block.h"

namespace Scratch
{

Block::Block()
{
	Item::updateItem();
}

bool Block::inConnectorActivationRange(const QPointF& position, const int& offset)
{
	return position.y() > offset - s_connectorActivationRange
		&& position.y() < offset + s_connectorActivationRange;
}

Item::Type Block::itemType() const
{
	return Type::Block;
}

QRectF Block::boundingRect() const
{
	return QRectF(
			0, -s_connectorActivationRange,
			m_width, m_height + 2 * s_connectorActivationRange)
		| childrenBoundingRect();
}

void Block::setPredecessorsReference(Block** reference)
{
	m_predecessorsReference = reference;
}

void Block::setSuccessor(Block* successor)
{
	m_successor = successor;
}

void Block::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(outline);
}

void Block::drawConnector(QPolygon& polygon, int x, int y, bool reverse) const
{
	std::array<QPoint, 4> points{{
		QPoint(
			x + s_margin,
			y),
		QPoint(
			x + s_margin + s_connectorMidsegmentOffset,
			y + s_connectorHeight),
		QPoint(
			x + s_margin + s_connectorWidth - s_connectorMidsegmentOffset,
			y + s_connectorHeight),
		QPoint(
			x + s_margin + s_connectorWidth,
			y)}};

	auto addPoint = [&](const auto& point)
		{
			polygon << point;
		};

	if (!reverse)
		std::for_each(points.cbegin(), points.cend(), addPoint);
	else
		std::for_each(points.crbegin(), points.crend(), addPoint);
}

void Block::drawOutline(QPolygon& polygon) const
{
	polygon << QPoint(m_width, m_height);
	drawConnector(polygon, 0, m_height, true);
	polygon	<< QPoint(0, m_height);

	polygon	<< QPoint(0, 0);
	drawConnector(polygon, 0, 0);
	polygon << QPoint(m_width, 0);
}

int Block::defaultHeight()
{
	return Item::defaultHeight() + 2 * s_connectorHeight;
}

bool Block::updateItem()
{
	if (!m_successor)
		return false;

	const auto dy = pos().y() + m_height - m_successor->pos().y();

	updateSuccessorPositions(dy);

	return false;
}

void Block::updateSuccessorPositions(int dy) const
{
	if (!m_successor)
		return;

	const auto oldPosition = m_successor->pos();

	m_successor->setPos(m_successor->pos() + QPointF(0, dy));

	if (m_successor->pos() != oldPosition)
		m_successor->updateSuccessorPositions(dy);
}

void Block::addAbove(Block& block)
{
	*m_predecessorsReference = &block;

	block.setPredecessorsReference(m_predecessorsReference);
	block.setSuccessor(this);

	m_predecessorsReference = &block.m_successor;

	block.setPos(pos());
	block.updateSuccessorPositions(block.m_height);

	addItem(block);
}

void Block::addBelow(Block& block)
{
	block.setPredecessorsReference(&m_successor);
	block.setSuccessor(m_successor);

	m_successor = &block;

	block.setPos(pos() + QPointF(0, m_height));
	block.updateSuccessorPositions(block.m_height);

	addItem(block);
}

void Block::remove()
{
	updateSuccessorPositions(-m_height);

	if (m_successor)
		m_successor->setPredecessorsReference(m_predecessorsReference);

	*m_predecessorsReference = m_successor;

	if (m_parent)
		m_parent->updateItem();

	Item::remove();
}

bool Block::isSelfOrAncestor(Block &block)
{
	for (QGraphicsItem* ancestor = this; ancestor; ancestor = ancestor->parentItem())
		if (ancestor == &block)
			return true;

	return false;
}

void Block::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos().toPoint();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() != Item::Type::Block)
		return;

	Block& block = *dynamic_cast<Block*>(&item);

	if (isSelfOrAncestor(block))
		return;

	if (!inConnectorActivationRange(position, 0) && !inConnectorActivationRange(position, m_height))
		return;

	if (item.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);
}

void Block::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	const auto oldHeight = m_height;

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

	auto& block = *dynamic_cast<Block*>(item);

	if (inConnectorActivationRange(position, 0))
		addAbove(block);
	else if (inConnectorActivationRange(position, oldHeight))
		addBelow(block);
}

void Block::print(std::ostream& stream, unsigned indentationDepth) const
{
	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

} // namespace Scratch
