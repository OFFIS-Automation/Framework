#ifndef PASSBLOCK_H
#define PASSBLOCK_H

#include "Block.h"

namespace Scratch
{

class PassBlock : public Block
{
	public:
		PassBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);
};

} // namespace Scratch

#endif // PASSBLOCK_H
