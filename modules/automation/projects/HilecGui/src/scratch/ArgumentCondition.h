#ifndef ARGUMENTCONDITION_H
#define ARGUMENTCONDITION_H

#include "ArgumentItem.h"
#include "Condition.h"

namespace Scratch
{

class ArgumentCondition : public ArgumentItem, public Condition
{
	public:
		ArgumentCondition(const std::string& name);

		Item& clone() const;

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);
		void print(std::ostream& stream) const;

	protected:
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		virtual void dropEvent(QGraphicsSceneDragDropEvent* event);

		QPoint resizeBy(int dx, int dy, const QPoint& triggerPosition);
};

} // namespace Scratch


#endif // ARGUMENTCONDITION_H
