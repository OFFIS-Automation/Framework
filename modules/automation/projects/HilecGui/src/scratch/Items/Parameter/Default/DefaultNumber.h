#ifndef DEFAULTNUMBER_H
#define DEFAULTNUMBER_H

#include <string>

#include <QGraphicsTextItem>

#include "../Parameter.h"
#include "../Number.h"

namespace Scratch
{

class DefaultNumber : public Number
{
	private:
		class NumberTextItem : public QGraphicsTextItem
		{
			public:
				NumberTextItem(Parameter& parent);

			protected:
				void keyPressEvent(QKeyEvent* event);

			private:
				Parameter& m_parent;
		};

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
