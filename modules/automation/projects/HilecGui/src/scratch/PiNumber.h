#ifndef PINUMBER_H
#define PINUMBER_H

#include "Number.h"

namespace Scratch
{

class PiNumber : public Number
{
	public:
		PiNumber();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Item& clone() const;
		void print(std::ostream& stream) const;
};

} // namespace Scratch

#endif // PINUMBER_H
