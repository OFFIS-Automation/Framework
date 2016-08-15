#ifndef SCRATCHCONDITION_H
#define SCRATCHCONDITION_H

#include <ostream>

#include "Item.h"

namespace Scratch
{

class Parameter : virtual public Item
{
	public:
		static const int s_defaultWidth = 80;

	public:
		QRectF boundingRect() const final;

		void remove();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
		virtual void drawOutline(QPolygon &polygon) const = 0;
		virtual void print(std::ostream& stream) const = 0;

		Parameter** m_parentReference{nullptr};

	protected:
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		virtual void dropEvent(QGraphicsSceneDragDropEvent* event);
};

std::ostream& operator<<(std::ostream& stream, const Scratch::Parameter& parameter);

} // namespace Scratch

#endif // SCRATCHCONDITION_H


