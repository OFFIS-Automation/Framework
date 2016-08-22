#ifndef DEFAULTPOINT_H
#define DEFAULTPOINT_H

#include "../Point.h"

namespace Scratch
{

class DefaultPoint : public Point
{
	public:
		DefaultPoint();

		Item& clone() const;
		void print(std::ostream& stream) const;

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

} // namespace Scratch

#endif // DEFAULTPOINT_H
