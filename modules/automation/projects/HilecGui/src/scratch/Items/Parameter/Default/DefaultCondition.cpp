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

	m_parent.updateItem();
}

DefaultCondition::DefaultCondition(const bool enable)
	: m_text(*this)
{
	m_textStyle.setBrush(Qt::gray);
	m_fillStyle = Qt::white;
	m_font.setBold(true);

	m_text.setDefaultTextColor(m_textStyle.color());
	m_text.setFont(m_font);
	m_text.setEnabled(enable);

	updateItem();

	setAcceptDrops(false);
}

Item& DefaultCondition::clone() const
{
	auto& item = *(new DefaultCondition());

	item.m_text.setEnabled(true);

	return item;
}

void DefaultCondition::print(std::ostream& stream) const
{
	stream << m_text.toPlainText().toStdString();
}

bool DefaultCondition::updateItem()
{
	m_text.setPos(
		m_width / 2 - m_text.boundingRect().width() / 2,
		m_height / 2 - m_text.boundingRect().height() / 2);

	return false;
}

void DefaultCondition::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->ignore();
}

} // namespace Scratch
