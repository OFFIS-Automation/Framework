#include "Condition.h"

namespace Scratch
{

Item::Type Condition::itemType() const
{
	return Type::Condition;
}

void Condition::drawOutline(QPainterPath& path) const
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

bool Condition::updateItem()
{
	m_horizontalMargin = m_height / 2;

	return true;
}

} // namespace Scratch
