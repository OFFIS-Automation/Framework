#include "Number.h"

namespace Scratch
{

Number::Number(int width, int height)
	: Parameter(width, height)
{}

Item::Type Number::itemType() const
{
	return Type::Number;
}

void Number::drawOutline(QPolygon &polygon, const QRect& area)
{
	const auto tipHeight = area.height() / 2;

	polygon
		<< area.topLeft() + QPoint(0, 0)
		<< area.topLeft() + QPoint(area.width(), 0)
		<< area.topLeft() + QPoint(area.width(), area.height())
		<< area.topLeft() + QPoint(0, area.height());
}

void Number::drawOutline(QPolygon& polygon)
{
	drawOutline(polygon, QRect(0, 0, m_width, m_height));
}

} // namespace Scratch
