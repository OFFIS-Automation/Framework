#include "Point.h"

namespace Scratch
{

Item::Type Point::itemType() const
{
	return Type::Point;
}

void Point::drawOutline(QPainterPath& path) const
{
	const auto tipHeight = m_height / 2;

	QPolygon polygon;

	polygon
		<< QPoint(0, tipHeight)
		<< QPoint(tipHeight, 0)
		<< QPoint(m_width - tipHeight, 0)
		<< QPoint(m_width, tipHeight)
		<< QPoint(m_width - tipHeight, m_height)
		<< QPoint(tipHeight, m_height)
		<< QPoint(0, tipHeight);

	path.addPolygon(polygon);
}

bool Point::updateItem()
{
	m_horizontalMargin = m_height / 2;

	return true;
}

} // namespace Scratch
