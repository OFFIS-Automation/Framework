#include <stdexcept>
#include <ostream>

#include <QPainter>
#include <QGraphicsScene>

#include "WhileBlock.h"

namespace Scratch
{

WhileBlock::WhileBlock(bool enable)
	: ControlFlowBlock("while", 1),
	  m_body{m_bodies.at(0)}
{
	addArgument("", Item::Type::Condition, enable);
}

void WhileBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget)
{
	ControlFlowBlock::paint(painter, style, widget);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(QRectF(m_horizontalMargin, s_margin,
		defaultHeight() - s_margin / 2, m_headerHeight - 2 * s_margin),
		Qt::AlignLeft | Qt::AlignVCenter, "while");
}

Block& WhileBlock::clone() const
{
	return *(new WhileBlock(true));
}

void WhileBlock::print(std::ostream& stream, unsigned indentationDepth = 0) const
{
	// while

	printIndentation(stream, indentationDepth);

	ArgumentItem::print(stream);

	stream << ":" << std::endl;

	if (m_body.block)
		m_body.block->print(stream, indentationDepth + 1);
	else
	{
		printIndentation(stream, indentationDepth + 1);
		stream << "pass" << std::endl;
	}

	Block::print(stream, indentationDepth);
}

} // namespace Scratch
