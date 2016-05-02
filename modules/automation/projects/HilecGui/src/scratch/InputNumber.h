#ifndef INPUTNUMBER_H
#define INPUTNUMBER_H

#include <QGraphicsTextItem>

#include "Number.h"

namespace Scratch
{

class InputNumber : public Number
{
	public:
		InputNumber();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Item& clone() const;
		void print(std::ostream& stream) const;

	private:
		QGraphicsTextItem m_text;
};

} // namespace Scratch

#endif // INPUTNUMBER_H
