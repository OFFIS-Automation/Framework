#include "TrueCondition.h"

#include <QPainter>

namespace Scratch
{

TrueCondition::TrueCondition()
	: Condition(80, 30)
{}

void TrueCondition::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(outline);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(boundingRect(), Qt::AlignCenter, "True");
}

Item& TrueCondition::clone() const
{
	return *(new TrueCondition());
}

void TrueCondition::print(std::ostream& stream) const
{
	stream << "True";
}

} // namespace Scratch
