#include <stdexcept>
#include <ostream>

#include <QPainter>
#include <QGraphicsScene>

#include "WhileBlock.h"

namespace Scratch
{

WhileBlock::WhileBlock()
	: ControlFlowBlock(100, 100),
	  m_body{m_bodies.at(0)}
{
	setAcceptDrops(true);
}

void WhileBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget)
{
	ControlFlowBlock<1>::paint(painter, style, widget);

	painter->setPen(m_textStyle);
	painter->drawText(QRectF(10, 10, m_width - 20, s_shaftHeight), "while");
}

Block& WhileBlock::clone() const
{
	return *(new WhileBlock());
}

void WhileBlock::print(std::ostream& stream, unsigned indentationDepth = 0) const
{
	if (!m_condition)
		throw std::invalid_argument("Not all conditions are defined.");

	if (!m_body.block)
		throw std::invalid_argument("Not all loop bodies are defined.");

	// while
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "while " << *m_condition << ":\n";

	m_body.block->print(stream, indentationDepth + 1);

	// next
	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

} // namespace Scratch
