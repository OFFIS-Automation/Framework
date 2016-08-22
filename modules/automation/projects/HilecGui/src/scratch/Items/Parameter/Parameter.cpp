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
		*m_parentReference = nullptr;
		m_parent->updateItem();
	}

	Item::remove();
}

void Parameter::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPainterPath outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPath(outline);
}

void Parameter::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	event->ignore();
}

void Parameter::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	event->ignore();
}

std::ostream& operator<<(std::ostream& stream, const Scratch::Parameter& parameter)
{
	parameter.print(stream);

	return stream;
}

} // namespace Scratch
