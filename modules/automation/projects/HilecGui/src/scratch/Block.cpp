#include <stdexcept>
#include <ostream>

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QApplication>
#include <QMimeData>
#include <QByteArray>
#include <QDrag>
#include <QWidget>

#include "Block.h"
#include "ProgramScene.h"

namespace Scratch
{

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

void Block::resizeBy(int dx, int dy)
{
	const auto actualDx = std::max(m_width + dx, s_defaultWidth) - m_width;
	const auto actualDy = std::max(m_height + dy, s_defaultHeight)- m_height;

	if (!actualDx && !actualDy)
		return;

	m_width += actualDx;
	m_height += actualDy;

	update(boundingRect());

	updateSuccessorPositions(actualDx, actualDy);

	if (m_parent)
		m_parent->resizeBy(actualDx, actualDy);
}

void Block::updateSuccessorPositions(int dx, int dy)
{
	for (Block* currentBlock = m_successor; currentBlock; currentBlock = currentBlock->m_successor)
		currentBlock->moveBy(dx, dy);
}

void Block::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length()
			< QApplication::startDragDistance())
		return;

	QMimeData* mimeData = new QMimeData;

	Block* item = this;
	QByteArray byteArray(reinterpret_cast<char*>(&item), sizeof(Block*));
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
		m_parent->resizeBy(0, block.m_height);

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
		m_parent->resizeBy(0, -m_height);

	if (m_successor)
		m_successor->setPredecessorsReference(m_predecessorsReference);

	*m_predecessorsReference = m_successor;

	setParent(nullptr);
}

} // namespace Scratch
