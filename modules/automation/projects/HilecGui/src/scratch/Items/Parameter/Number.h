#ifndef NUMBER_H
#define NUMBER_H

#include "Parameter.h"

namespace Scratch
{

class Number : public Parameter
{
	public:
		static void drawOutline(QPolygon& polygon, const QRect& area);

	public:
		Item::Type itemType() const final;

		void drawOutline(QPainterPath& path) const;
};

} // namespace Scratch

#endif // NUMBER_H
