#include "NumberTextItem.h"

#include <QKeyEvent>
#include <QTextCursor>

namespace Scratch
{

NumberTextItem::NumberTextItem(Parameter& parent)
	: QGraphicsTextItem("0"),
	  m_parent(parent)
{
	setParentItem(&m_parent);
}

void NumberTextItem::keyPressEvent(QKeyEvent* event)
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

}
