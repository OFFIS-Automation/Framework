#include "Condition.h"

namespace Scratch
{

Item::Type Condition::itemType() const
{
	return Type::Condition;
}

void Condition::drawOutline(QPolygon &poylgon, const QRect& area)
{
	const auto tipHeight = area.height() / 2;

	poylgon	<< (area.topLeft() + QPoint(0, tipHeight))
		<< (area.topLeft() + QPoint(tipHeight, 0))
		<< (area.topLeft() + QPoint(area.width() - tipHeight, 0))
		<< (area.topLeft() + QPoint(area.width(), tipHeight))
		<< (area.topLeft() + QPoint(area.width() - tipHeight, area.height()))
		<< (area.topLeft() + QPoint(tipHeight, area.height()));
}

void Condition::drawOutline(QPolygon &poylgon) const
{
	drawOutline(poylgon, QRect(0, 0, m_width, m_height));
}

} // namespace Scratch
