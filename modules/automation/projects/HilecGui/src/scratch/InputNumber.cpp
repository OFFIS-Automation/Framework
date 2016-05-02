#include "InputNumber.h"

#include <QPainter>

namespace Scratch
{

InputNumber::InputNumber()
	: Number(80, 30),
	  m_text("0")
{
	m_text.setParentItem(this);
	m_text.setPos(m_width / 2 - m_text.boundingRect().width() / 2,
		m_height / 2 - m_text.boundingRect().height() / 2);

	m_text.setDefaultTextColor(m_textStyle.color());
	m_text.setFont(m_font);
}

void InputNumber::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(outline);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
}

Item& InputNumber::clone() const
{
	auto& inputNumber = *(new InputNumber());

	inputNumber.m_text.setTextInteractionFlags(Qt::TextEditable);

	return inputNumber;
}

void InputNumber::print(std::ostream& stream) const
{
	stream << m_text.toPlainText().toStdString();
}

} // namespace Scratch
