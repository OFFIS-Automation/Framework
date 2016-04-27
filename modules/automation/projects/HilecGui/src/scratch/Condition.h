#ifndef CONDITION_H
#define CONDITION_H

#include "Parameter.h"

namespace Scratch
{

class Condition : public Parameter
{
	public:
		static void drawOutline(QPolygon& poylgon, const QRect& area);

	public:
		Condition(int width, int height);

		Item::Type itemType() const final;

		void drawOutline(QPolygon &poylgon);
};

} // namespace Scratch

#endif // CONDITION_H
