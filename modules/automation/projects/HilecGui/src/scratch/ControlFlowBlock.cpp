#include "ControlFlowBlock.h"

#include <QGraphicsScene>

namespace Scratch
{

void ControlFlowBlock::addBody(Block& block, Block*& body, const QPoint &offset)
{
	const auto dHeight = block.m_height - !body * (defaultBodyHeight());

	block.setPredecessorsReference(&body);
	block.setSuccessor(body);
	block.setParent(this);

	body = &block;

	block.setPos(offset);
	resizeBy(0, dHeight, offset);

	scene()->addItem(&block);
}

} // namespace Scratch
