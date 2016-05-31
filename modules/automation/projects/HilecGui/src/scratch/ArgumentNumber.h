#ifndef ARGUMENTNUMBER_H
#define ARGUMENTNUMBER_H

#include "ArgumentItem.h"
#include "Number.h"

namespace Scratch
{

class ArgumentNumber : public ArgumentItem, public Number
{
	public:
		ArgumentNumber(const std::string& name);

		Item& clone() const;

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);
		void print(std::ostream& stream) const;

	protected:
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		virtual void dropEvent(QGraphicsSceneDragDropEvent* event);

		QPoint resizeBy(int dx, int dy, const QPoint& triggerPosition);
};

} // namespace Scratch

#endif // ARGUMENTNUMBER_H
