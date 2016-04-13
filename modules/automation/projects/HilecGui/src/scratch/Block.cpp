#include <stdexcept>
#include <ostream>
#include <array>

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QApplication>
#include <QMimeData>
#include <QByteArray>
#include <QDrag>
#include <QWidget>
#include <QGraphicsSceneDragDropEvent>

#include "Block.h"
#include "ProgramScene.h"

namespace Scratch
{

Block& Block::unpackBlock(const QGraphicsSceneDragDropEvent& event)
{
	auto& byteArray = event.mimeData()->data("Block");

	return **reinterpret_cast<Block**>(byteArray.data());
}

bool Block::inConnectorActivationRange(const QPointF& position, const int& offset)
{
	return position.y() > offset - s_connectorActivationRange
		&& position.y() < offset + s_connectorActivationRange;
}

Block::Block(const int defaultWidth, const int defaultHeight)
	: m_defaultWidth(defaultWidth),
	  m_defaultHeight(defaultHeight),
	  m_width(defaultWidth),
	  m_height(defaultHeight)
{}

QRectF Block::boundingRect() const
{
	return QRectF(
		0, -s_connectorActivationRange,
		m_width, m_height + 2 * s_connectorActivationRange);
}

void Block::setPredecessorsReference(Block** reference)
{
	m_predecessorsReference = reference;
}

void Block::setSuccessor(Block* successor)
{
	m_successor = successor;
}

void Block::setParent(Block* parent)
{
	m_parent = parent;

	setParentItem(parent);
}

void Block::addConnector(QPolygon& polygon, int x, int y, bool reverse) const
{
	const auto connectorWidthOffset = (s_connectorWidth - s_connectorMidsegmentWidth) / 2;

	std::array<QPoint, 4> points{{
		QPoint(
			x + s_connectorMargin,
			y),
		QPoint(
			x + s_connectorMargin + connectorWidthOffset,
			y + s_connectorHeight),
		QPoint(
			x + s_connectorMargin + s_connectorWidth - connectorWidthOffset,
			y + s_connectorHeight),
		QPoint(
			x + s_connectorMargin + s_connectorWidth,
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

void Block::resizeBy(int dx, int dy, const QPointF&)
{
	const auto actualDx = std::max(m_width + dx, m_defaultWidth) - m_width;
	const auto actualDy = std::max(m_height + dy, m_defaultHeight)- m_height;

	if (!actualDx && !actualDy)
		return;

	prepareGeometryChange();

	m_width += actualDx;
	m_height += actualDy;

	updateSuccessorPositions(actualDx, actualDy);

	if (m_parent)
		m_parent->resizeBy(actualDx, actualDy, pos());
}

void Block::updateSuccessorPositions(int dx, int dy) const
{
	for (Block* currentBlock = m_successor; currentBlock; currentBlock = currentBlock->m_successor)
		currentBlock->moveBy(dx, dy);
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->accept();
}

void Block::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	event->accept();

	if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length()
			< QApplication::startDragDistance())
		return;

	QMimeData* mimeData = new QMimeData;

	auto* block = this;
	QByteArray byteArray(reinterpret_cast<char*>(&block), sizeof(Block*));
	mimeData->setData("Block", byteArray);

	QDrag* drag = new QDrag(event->widget());
	drag->setMimeData(mimeData);
	drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void Block::addBlock(Block& block)
{
	block.setParent(m_parent);
	block.setPos(pos());

	if (m_parent)
		m_parent->resizeBy(0, block.m_height, pos());

	scene()->addItem(&block);
}

void Block::addAbove(Block& block)
{
	*m_predecessorsReference = &block;

	block.setPredecessorsReference(m_predecessorsReference);
	block.setSuccessor(this);

	m_predecessorsReference = &block.m_successor;

	addBlock(block);

	block.updateSuccessorPositions(0, block.m_height);
}

void Block::addBelow(Block& block)
{
	block.setPredecessorsReference(&m_successor);
	block.setSuccessor(m_successor);

	m_successor = &block;

	addBlock(block);

	updateSuccessorPositions(0, m_height);
}

void Block::remove()
{
	scene()->removeItem(this);

	updateSuccessorPositions(0, -m_height);

	if (m_parent)
		m_parent->resizeBy(0, -m_height, pos());

	if (m_successor)
		m_successor->setPredecessorsReference(m_predecessorsReference);

	*m_predecessorsReference = m_successor;

	setParent(nullptr);
}

bool Block::isSelfOrAncestor(Block &block)
{
	for (QGraphicsItem* ancestor = this; ancestor; ancestor = ancestor->parentItem())
		if (ancestor == &block)
			return true;

	return false;
}

} // namespace Scratch
