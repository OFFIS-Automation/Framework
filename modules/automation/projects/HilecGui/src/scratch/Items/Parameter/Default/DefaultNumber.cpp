#include "DefaultNumber.h"

#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QKeyEvent>
#include <QTextCursor>

namespace Scratch
{

DefaultNumber::NumberTextItem::NumberTextItem(Parameter& parent)
	: QGraphicsTextItem("0"),
	  m_parent(parent)
{
	setParentItem(&m_parent);
}

void DefaultNumber::NumberTextItem::keyPressEvent(QKeyEvent* event)
{
	if ((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
		|| (event->key() == Qt::Key_Minus && textCursor().position() == 0
			&& !toPlainText().contains('-'))
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

	m_parent.updateItem();
}

DefaultNumber::DefaultNumber(const bool enable)
	: m_text(*this)
{
	m_textStyle.setBrush(Qt::gray);
	m_fillStyle = Qt::white;
	m_font.setBold(true);

	m_text.setDefaultTextColor(m_textStyle.color());
	m_text.setFont(m_font);

	if (enable)
		m_text.setTextInteractionFlags(Qt::TextEditable);

	updateItem();

	setAcceptDrops(false);
}

Item& DefaultNumber::clone() const
{
	auto& item = *(new DefaultNumber());

	item.m_text.setTextInteractionFlags(Qt::TextEditable);

	return item;
}

void DefaultNumber::print(std::ostream& stream) const
{
	stream << m_text.toPlainText().toStdString();
}

bool DefaultNumber::updateItem()
{
	prepareGeometryChange();

	auto oldWidth = m_width;

	m_width = m_text.boundingRect().width() + 2 * m_horizontalMargin;

	m_text.setPos(m_horizontalMargin, m_height / 2 - m_text.boundingRect().height() / 2);

	auto updated = oldWidth != m_width;

	if (m_parent && updated)
		m_parent->updateItem();

	return updated;
}

void DefaultNumber::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->ignore();
}

} // namespace Scratch
