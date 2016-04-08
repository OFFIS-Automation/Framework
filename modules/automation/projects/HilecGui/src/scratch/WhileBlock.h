#ifndef SCRATCHWHILEBLOCK_H
#define SCRATCHWHILEBLOCK_H

#include "ControlFlowBlock.h"

namespace Scratch
{

class WhileBlock : public ControlFlowBlock
{
	private:
		int m_headerHeight = s_shaftHeight;

	public:
		WhileBlock();

		QRectF boundingRect() const;
		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Block& clone();
		void print(std::ostream& stream, unsigned indentationDepth);

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

	private:
		void addBody(Block& block);

		Condition* m_condition{nullptr};
		Block* m_body{nullptr};
};

} // namespace Scratch

#endif // SCRATCHWHILEBLOCK_H
