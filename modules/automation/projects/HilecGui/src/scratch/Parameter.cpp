#include "Parameter.h"

#include <QGraphicsSceneEvent>

namespace Scratch
{

Parameter::Parameter(const int width, const int height)
	: Item(width, height)
{}

QRectF Parameter::boundingRect() const
{
	return QRect(0, 0, m_width, m_height);
}

void Parameter::remove()
{
	if (m_parent)
		m_parent->resizeBy(-m_width, -m_height, pos());

	Item::remove();
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
