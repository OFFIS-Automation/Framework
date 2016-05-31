#include "PiNumber.h"

#include <QPainter>

namespace Scratch
{

PiNumber::PiNumber()
	: Item(80, 30)
{}

void PiNumber::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(outline);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(boundingRect(), Qt::AlignCenter, "Pi");
}

Item& PiNumber::clone() const
{
	return *(new PiNumber());
}

void PiNumber::print(std::ostream& stream) const
{
	stream << "3.14159265359";
}

} // namespace Scratch
