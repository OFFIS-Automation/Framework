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

	path.moveTo(2 * tipHeight, 0);
	path.arcTo(0, 0, 2 * tipHeight, m_height, 90.0, 180.0);
	path.lineTo(m_width - 2 * tipHeight, m_height);
	path.arcTo(m_width - 2 * tipHeight, 0, 2 * tipHeight, m_height, 270.0, 180.0);
	path.closeSubpath();
}

bool Point::updateItem()
{
	m_horizontalMargin = m_height / 2;

	return true;
}

} // namespace Scratch
