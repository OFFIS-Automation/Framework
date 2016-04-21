#include <stdexcept>
#include <ostream>

#include <QPainter>
#include <QGraphicsScene>

#include "IfElseBlock.h"

namespace Scratch
{

IfElseBlock::IfElseBlock()
	: ControlFlowBlock(100, 150),
	  m_trueBody{m_bodies.at(0)},
	  m_falseBody{m_bodies.at(1)}
{
	setAcceptDrops(true);
}

void IfElseBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget)
{
	ControlFlowBlock<2>::paint(painter, style, widget);

	painter->setPen(m_textStyle);
	painter->drawText(QRectF(10, 10, m_width - 20, s_shaftHeight), "if");
	painter->drawText(QRectF(
		10, m_headerHeight + m_trueBody.height + 10, m_width - 20, s_shaftHeight), "else");
}

Block& IfElseBlock::clone() const
{
	return *(new IfElseBlock());
}

void IfElseBlock::print(std::ostream& stream, unsigned indentationDepth = 0) const
{
	if (!m_condition)
		throw std::invalid_argument("Not all conditions are defined.");

	if (!m_trueBody.block || !m_falseBody.block)
		throw std::invalid_argument("Not all if bodies are defined.");

	// if

	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "if " << *m_condition << ":\n";

	m_trueBody.block->print(stream, indentationDepth + 1);

	// else

	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "else:\n";

	m_falseBody.block->print(stream, indentationDepth + 1);

	// next

	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

} // namespace Scratch
