#ifndef POINT_H
#define POINT_H

#include "Parameter.h"

namespace Scratch
{

class Point : public Parameter
{
	public:
		Item::Type itemType() const final;

		void drawOutline(QPainterPath& path) const;

	protected:
		virtual bool updateItem();
};

} // namespace Scratch

#endif // POINT_H
