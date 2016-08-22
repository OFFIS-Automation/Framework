#include "DefaultPoint.h"

#include <QGraphicsSceneMouseEvent>
#include <QString>

namespace Scratch
{

DefaultPoint::DefaultPoint()
{
	m_textStyle.setBrush(Qt::lightGray);
	m_fillStyle = Qt::white;
	m_font.setBold(true);

	setAcceptDrops(false);
}

Item& DefaultPoint::clone() const
{
	return *(new DefaultPoint());
}

void DefaultPoint::print(std::ostream& stream) const
{
	throw std::invalid_argument("Not all points are defined.");
}

void DefaultPoint::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->ignore();
}

} // namespace Scratch
