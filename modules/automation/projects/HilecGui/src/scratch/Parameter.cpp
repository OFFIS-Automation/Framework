#include "Parameter.h"

#include <QGraphicsSceneEvent>
#include <QPainter>

namespace Scratch
{

QRectF Parameter::boundingRect() const
{
	return QRect(0, 0, m_width, m_height);
}

void Parameter::remove()
{
	if (m_parent)
	{
		m_parent->resizeBy(-m_width, -m_height, pos().toPoint());
		*m_parentReference = nullptr;
	}

	Item::remove();
}

void Parameter::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(outline);
}

void Parameter::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	event->accept();
	event->setDropAction(Qt::IgnoreAction);
}

void Parameter::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	event->accept();
}

std::ostream& operator<<(std::ostream& stream, const Scratch::Parameter& parameter)
{
	parameter.print(stream);

	return stream;
}

} // namespace Scratch
