#include "DefaultNumber.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QKeyEvent>
#include <QTextCursor>

namespace Scratch
{

DefaultNumber::NumberTextItem::NumberTextItem(DefaultNumber& parent)
	: QGraphicsTextItem("0"),
	  m_parent(parent)
{
	setParentItem(&m_parent);
}

void DefaultNumber::NumberTextItem::keyPressEvent(QKeyEvent* event)
{
	if ((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
		|| (event->key() == Qt::Key_Period && !toPlainText().contains('.'))
		|| event->key() == Qt::Key_Backspace
		|| event->key() == Qt::Key_Delete)
			QGraphicsTextItem::keyPressEvent(event);

	if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
	{
		auto cursor = textCursor();
		cursor.movePosition(event->key() == Qt::Key_Left?
			QTextCursor::Left : QTextCursor::Right);

		setTextCursor(cursor);
	}

	updatePosition();
}

void DefaultNumber::NumberTextItem::updatePosition()
{
	setPos(m_parent.m_width / 2 - boundingRect().width() / 2,
		m_parent.m_height / 2 - boundingRect().height() / 2);
}

DefaultNumber::DefaultNumber(const bool enable)
	: Number(80, 30),
	  m_text(*this)
{
	m_textStyle.setBrush(Qt::lightGray);
	m_fillStyle = Qt::white;
	m_font.setBold(true);

	m_text.setDefaultTextColor(m_textStyle.color());
	m_text.setFont(m_font);

	if (enable)
		m_text.setTextInteractionFlags(Qt::TextEditable);

	m_text.updatePosition();
}

void DefaultNumber::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(outline);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
}

Item& DefaultNumber::clone() const
{
	auto& inputNumber = *(new DefaultNumber());

	inputNumber.m_text.setTextInteractionFlags(Qt::TextEditable);

	return inputNumber;
}

void DefaultNumber::print(std::ostream& stream) const
{
	stream << m_text.toPlainText().toStdString();
}

void DefaultNumber::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->ignore();
}

} // namespace Scratch
