#ifndef ARGUMENTBLOCK_H
#define ARGUMENTBLOCK_H

#include "Block.h"
#include "ArgumentItem.h"

namespace Scratch
{

class ArgumentBlock : public Block, public ArgumentItem
{
	public:
		ArgumentBlock(const std::string& name);

		Item& clone() const;

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);
		void print(std::ostream& stream, unsigned indentationDepth) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		QPoint resizeBy(int dx, int dy, const QPoint& triggerPosition);
};

} // namespace Scratch

#endif // ARGUMENTBLOCK_H
