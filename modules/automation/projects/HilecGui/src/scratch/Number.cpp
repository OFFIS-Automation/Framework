#include "Number.h"

#include <QPainter>

namespace Scratch
{

Item::Type Number::itemType() const
{
	return Type::Number;
}

void Number::drawOutline(QPolygon &polygon, const QRect& area)
{
	polygon
		<< area.topLeft() + QPoint(0, 0)
		<< area.topLeft() + QPoint(area.width(), 0)
		<< area.topLeft() + QPoint(area.width(), area.height())
		<< area.topLeft() + QPoint(0, area.height());
}

void Number::drawOutline(QPolygon& polygon) const
{
	drawOutline(polygon, QRect(0, 0, m_width, m_height));
}

} // namespace Scratch
