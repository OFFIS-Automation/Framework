#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include "PassBlock.h"

namespace Scratch
{

PassBlock::PassBlock()
	: Item(s_defaultWidth, s_defaultHeight)
{
	setAcceptDrops(true);
}

void PassBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon polygon;
	drawOutline(polygon);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(polygon);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(boundingRect(), Qt::AlignCenter, "Pass");
}

Block& PassBlock::clone() const
{
	return *(new PassBlock());
}

void PassBlock::print(std::ostream& stream, unsigned indentationDepth = 0) const
{
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "pass\n";

	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

} // namespace Scratch
