#include "TrueCondition.h"

#include <QPainter>

namespace Scratch
{

TrueCondition::TrueCondition()
	: Condition(100, 30)
{}

void TrueCondition::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	const auto tipHeight = m_height / 2;

	QPolygon polygon;

	polygon	<< QPoint(0, tipHeight)
		<< QPoint(tipHeight, 0)
		<< QPoint(m_width - tipHeight, 0)
		<< QPoint(m_width, tipHeight)
		<< QPoint(m_width - tipHeight, m_height)
		<< QPoint(tipHeight, m_height);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

	painter->setPen(m_textStyle);
	painter->drawText(QRectF(m_width / 2 - 10, 10, m_width - 20, m_height - 20), "True");
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
