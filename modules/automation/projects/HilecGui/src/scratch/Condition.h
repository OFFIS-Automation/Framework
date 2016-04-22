#ifndef CONDITION_H
#define CONDITION_H

#include "Parameter.h"

namespace Scratch
{

class Condition : public Parameter
{
	public:
		static void drawOutline(QPolygon& poylgon, const int width,	const int height,
			const QPoint& position = QPoint());

	public:
		Condition(int width, int height);

		Item::Type itemType() const final;

		void drawOutline(QPolygon &poylgon);
};

} // namespace Scratch

#endif // CONDITION_H
