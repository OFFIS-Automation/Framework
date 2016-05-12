#include <stdexcept>
#include <ostream>

#include <QPainter>
#include <QGraphicsScene>

#include "WhileBlock.h"

namespace Scratch
{

WhileBlock::WhileBlock()
	: Item(s_defaultWidth, s_defaultHeight + 2 * s_defaultHeaderHeight),
	  ControlFlowBlock(1),
	  m_body{m_bodies.at(0)}
{
	setAcceptDrops(true);
}

void WhileBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget)
{
	ControlFlowBlock::paint(painter, style, widget);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(QRectF(s_margin, s_margin,
		s_shaftExtent - s_margin / 2, m_headerHeight - 2 * s_margin),
		Qt::AlignLeft | Qt::AlignVCenter, "while");
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
