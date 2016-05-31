#include "DefaultCondition.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QKeyEvent>
#include <QTextCursor>

namespace Scratch
{

DefaultCondition::ConditionTextItem::ConditionTextItem(DefaultCondition& parent)
	: QGraphicsTextItem("True"),
	  m_parent(parent)
{
	setParentItem(&m_parent);
}

void DefaultCondition::ConditionTextItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	setPlainText(toPlainText() == "True"? "False" : "True");

	updatePosition();
}

void DefaultCondition::ConditionTextItem::updatePosition()
{
	setPos(m_parent.m_width / 2 - boundingRect().width() / 2,
		m_parent.m_height / 2 - boundingRect().height() / 2);
}

DefaultCondition::DefaultCondition(const bool enable)
	: Item(60, 20),
	  m_text(*this)
{
	m_textStyle.setBrush(Qt::lightGray);
	m_fillStyle = Qt::white;
	m_font.setBold(true);

	m_text.setDefaultTextColor(m_textStyle.color());
	m_text.setFont(m_font);
	m_text.setEnabled(enable);

	m_text.updatePosition();
}

void DefaultCondition::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(outline);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
}

Item& DefaultCondition::clone() const
{
	auto& inputNumber = *(new DefaultCondition());

	inputNumber.m_text.setEnabled(true);

	return inputNumber;
}

void DefaultCondition::print(std::ostream& stream) const
{
	stream << m_text.toPlainText().toStdString();
}

void DefaultCondition::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->ignore();
}

} // namespace Scratch
