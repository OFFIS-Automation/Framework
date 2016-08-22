#include "Number.h"

#include <QPainter>

namespace Scratch
{

Item::Type Number::itemType() const
{
	return Type::Number;
}

void Number::drawOutline(QPainterPath& path) const
{
	QPolygon polygon;

	polygon
		<< QPoint(0, 0)
		<< QPoint(m_width, 0)
		<< QPoint(m_width, m_height)
		<< QPoint(0, m_height);

	path.addPolygon(polygon);
	path.closeSubpath();
}

} // namespace Scratch
