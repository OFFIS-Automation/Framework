#ifndef DEFAULTNUMBER_H
#define DEFAULTNUMBER_H

#include <string>

#include <QGraphicsTextItem>

#include "Number.h"

namespace Scratch
{

class DefaultNumber : public Number
{
	private:
		class NumberTextItem : public QGraphicsTextItem
		{
			public:
				NumberTextItem(DefaultNumber& parent);

				void updatePosition();

			protected:
				void keyPressEvent(QKeyEvent* event);

			private:
				DefaultNumber& m_parent;
		};

	public:
		DefaultNumber(const bool enable = false);

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Item& clone() const;
		void print(std::ostream& stream) const;

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event);

	private:
		NumberTextItem m_text;
};

} // namespace Scratch

#endif // DEFAULTNUMBER_H
