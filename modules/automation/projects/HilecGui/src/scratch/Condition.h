#ifndef SCRATCHCONDITION_H
#define SCRATCHCONDITION_H

#include <ostream>

#include "Item.h"

namespace Scratch
{

class Condition : public Item
{
	public:
		QRectF boundingRect() const final;

		Type itemType() const final;

		void remove();
		virtual void print(std::ostream& stream) const = 0;

	protected:
		Condition(int width, int height);

		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);
};

std::ostream& operator<<(std::ostream& stream, const Scratch::Condition& condition);

} // namespace Scratch

#endif // SCRATCHCONDITION_H


