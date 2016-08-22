#ifndef CONDITION_H
#define CONDITION_H

#include "Parameter.h"

namespace Scratch
{

class Condition : public Parameter
{
	public:
		Item::Type itemType() const final;

		void drawOutline(QPainterPath& path) const;

	protected:
		virtual bool updateItem();
};

} // namespace Scratch

#endif // CONDITION_H
