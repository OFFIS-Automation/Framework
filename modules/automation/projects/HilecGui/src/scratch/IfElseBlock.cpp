#include <stdexcept>
#include <ostream>

#include <QPainter>
#include <QGraphicsScene>

#include "IfElseBlock.h"

namespace Scratch
{

IfElseBlock::IfElseBlock(bool enable)
	: Item(s_defaultWidth, 2 * s_defaultHeight + 3 * s_defaultHeaderHeight),
	  ControlFlowBlock("if", 2),
	  m_trueBody{m_bodies.at(0)},
	  m_falseBody{m_bodies.at(1)}
{
	addArgument("", Item::Type::Condition, enable);
}

void IfElseBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget)
{
	ControlFlowBlock::paint(painter, style, widget);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);

	painter->drawText(QRectF(s_margin, s_margin,
		s_shaftExtent - s_margin / 2, m_headerHeight - 2 * s_margin),
		Qt::AlignLeft | Qt::AlignVCenter, "if");

	painter->drawText(QRectF(s_margin, m_headerHeight + m_trueBody.height + s_margin,
		s_shaftExtent - s_margin / 2, s_defaultHeaderHeight - 2 * s_margin),
		Qt::AlignLeft | Qt::AlignVCenter, "else");
}

Block& IfElseBlock::clone() const
{
	return *(new IfElseBlock());
}

void IfElseBlock::print(std::ostream& stream, unsigned indentationDepth = 0) const
{
	if (!m_trueBody.block || !m_falseBody.block)
		throw std::invalid_argument("Not all if bodies are defined.");

	// if

	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	ArgumentItem::print(stream);

	stream << ":\n";

	m_trueBody.block->print(stream, indentationDepth + 1);

	// else

	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "else:\n";

	m_falseBody.block->print(stream, indentationDepth + 1);

	// next

	Block::print(stream, indentationDepth);
}

} // namespace Scratch
