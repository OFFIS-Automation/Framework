#ifndef DEFAULTPOINT_H
#define DEFAULTPOINT_H

#include "../Point.h"

#include "NumberTextItem.h"

namespace Scratch
{

class DefaultPoint : public Point
{
	public:
		DefaultPoint(const bool enable);

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

		Item& clone() const;
		void print(std::ostream& stream) const;

		bool updateItem();

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event);

	private:
		NumberTextItem m_x, m_y;
};

} // namespace Scratch

#endif // DEFAULTPOINT_H
