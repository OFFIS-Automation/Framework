#include "DefaultPoint.h"

#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QPainter>

namespace Scratch
{

DefaultPoint::DefaultPoint(const bool enable)
:	m_x(*this),
	m_y(*this)
{
	m_textStyle.setBrush(Qt::gray);
	m_fillStyle = Qt::white;
	m_font.setBold(true);

	m_x.setDefaultTextColor(m_textStyle.color());
	m_x.setFont(m_font);

	m_y.setDefaultTextColor(m_textStyle.color());
	m_y.setFont(m_font);

	if (enable)
	{
		m_x.setTextInteractionFlags(Qt::TextEditable);
		m_y.setTextInteractionFlags(Qt::TextEditable);
	}

	updateItem();

	setAcceptDrops(false);
}

Item& DefaultPoint::clone() const
{
	return *(new DefaultPoint(true));
}

void DefaultPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
	QFontMetrics fontMetric(m_font);

	Point::paint(painter, item, widget);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);

	painter->drawText(QPoint(m_horizontalMargin + m_x.boundingRect().width()
		- fontMetric.width(",") / 2, s_margin + fontMetric.height()), ",");
}

void DefaultPoint::print(std::ostream& stream) const
{
	stream << "types.Point(" << m_x.toPlainText().toStdString() << ", "
		<< m_y.toPlainText().toStdString() << ")";
}

bool DefaultPoint::updateItem()
{
	prepareGeometryChange();

	auto oldWidth = m_width;

	m_width = m_x.boundingRect().width() + m_y.boundingRect().width() + 2 * m_horizontalMargin;

	m_x.setPos(m_horizontalMargin, m_height / 2 - m_x.boundingRect().height() / 2);
	m_y.setPos(
		m_width - m_horizontalMargin - m_y.boundingRect().width(),
		m_height / 2 - m_x.boundingRect().height() / 2);

	auto updated = oldWidth != m_width;

	if (m_parent && updated)
		m_parent->updateItem();

	return updated;
}

void DefaultPoint::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->ignore();
}

} // namespace Scratch
