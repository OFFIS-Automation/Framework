#include <stdexcept>
#include <ostream>

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
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
	m_width += dx;
	m_height += dy;

	updateSuccessorPositions(dx, dy);

	if (m_parent)
		m_parent->resizeBy(dx, dy);
}

void Block::updateSuccessorPositions(int dx, int dy)
{
	for (Block* currentBlock = m_successor; currentBlock; currentBlock = currentBlock->m_successor)
		currentBlock->moveBy(dx, dy);
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QMimeData* mimeData = new QMimeData;

	Block* item = this;
	QByteArray byteArray(reinterpret_cast<char*>(&item), sizeof(Block*));
	mimeData->setData("Block", byteArray);

	QDrag* drag = new QDrag(event->widget());
	drag->setMimeData(mimeData);
	drag->exec(Qt::CopyAction, Qt::CopyAction);
}

void Block::addBlock(Block& block)
{
	block.setParent(m_parent);

	block.setPos(pos());
	updateSuccessorPositions(0, block.m_height);

	if (m_parent)
		m_parent->resizeBy(0, block.m_height);

	scene()->addItem(&block);
}

void Block::addAbove(Block& block)
{
	*m_predecessorsReference = &block;

	block.setPredecessorsReference(m_predecessorsReference);
	block.setSuccessor(this);

	addBlock(block);

	moveBy(0, block.m_height);
}

void Block::addBelow(Block& block)
{
	block.setPredecessorsReference(&m_successor);
	block.setSuccessor(m_successor);

	m_successor = &block;

	addBlock(block);
}

} // namespace Scratch
