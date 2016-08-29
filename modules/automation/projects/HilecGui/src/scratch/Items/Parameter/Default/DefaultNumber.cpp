#include "DefaultNumber.h"

#include <QGraphicsSceneMouseEvent>

namespace Scratch
{

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
	return *(new DefaultNumber(true));
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
