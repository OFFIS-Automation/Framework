#ifndef ARGUMENTBLOCK_H
#define ARGUMENTBLOCK_H

#include "Block.h"
#include "ArgumentItem.h"

namespace Scratch
{

template <>
class Argument<Block> : public Block, public ArgumentItem
{
	public:
		Argument(const std::string& name);

		Item& clone() const;

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);
		virtual void print(std::ostream& stream, unsigned indentationDepth) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		bool updateItem();
};

} // namespace Scratch

#endif // ARGUMENTBLOCK_H
