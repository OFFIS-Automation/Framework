#include "TrueCondition.h"

#include <QPainter>

namespace Scratch
{

TrueCondition::TrueCondition()
	: Condition(100, 30)
{}

void TrueCondition::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon polygon;

	drawOutline(polygon, m_width, m_height);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

	painter->setPen(m_textStyle);
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
