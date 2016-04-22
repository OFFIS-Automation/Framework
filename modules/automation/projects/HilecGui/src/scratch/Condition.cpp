#include "Condition.h"

namespace Scratch
{

Condition::Condition(int width, int height)
	: Parameter(width, height)
{}

Item::Type Condition::itemType() const
{
	return Type::Condition;
}

void Condition::drawOutline(QPolygon &poylgon, const int width, const int height,
	const QPoint& position)
{
	const auto tipHeight = height / 2;

	poylgon	<< (position + QPoint(0, tipHeight))
		<< (position + QPoint(tipHeight, 0))
		<< (position + QPoint(width - tipHeight, 0))
		<< (position + QPoint(width, tipHeight))
		<< (position + QPoint(width - tipHeight, height))
		<< (position + QPoint(tipHeight, height));
}

void Condition::drawOutline(QPolygon &poylgon)
{
	drawOutline(poylgon, m_width, m_height);
}

} // namespace Scratch
