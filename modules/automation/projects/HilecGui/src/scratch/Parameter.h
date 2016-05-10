#ifndef SCRATCHCONDITION_H
#define SCRATCHCONDITION_H

#include <ostream>

#include "Item.h"

namespace Scratch
{

class Parameter : public Item
{
	public:
		QRectF boundingRect() const final;

		void remove();
		virtual void print(std::ostream& stream) const = 0;

		Parameter** m_parentReference{nullptr};

	protected:
		Parameter(int width, int height);

		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		virtual void dropEvent(QGraphicsSceneDragDropEvent* event);
};

std::ostream& operator<<(std::ostream& stream, const Scratch::Parameter& parameter);

} // namespace Scratch

#endif // SCRATCHCONDITION_H


