#ifndef DEFAULTNUMBER_H
#define DEFAULTNUMBER_H

#include <string>

#include "../Parameter.h"
#include "../Number.h"

#include "NumberTextItem.h"

namespace Scratch
{

class DefaultNumber : public Number
{
	public:
		DefaultNumber(const bool enable = false);

		Item& clone() const;
		void print(std::ostream& stream) const;

		bool updateItem();

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event);

	private:
		NumberTextItem m_text;
};

} // namespace Scratch

#endif // DEFAULTNUMBER_H
