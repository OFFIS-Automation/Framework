#ifndef TRUECONDITION_H
#define TRUECONDITION_H

#include "Condition.h"

namespace Scratch
{

class TrueCondition : public Condition
{
	public:
		TrueCondition();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Item& clone() const;
		void print(std::ostream& stream) const;
};

} // namespace Scratch

#endif // TRUECONDITION_H
