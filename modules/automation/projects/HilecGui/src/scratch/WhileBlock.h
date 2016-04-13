#ifndef SCRATCHWHILEBLOCK_H
#define SCRATCHWHILEBLOCK_H

#include "ControlFlowBlock.h"

namespace Scratch
{

class WhileBlock : public ControlFlowBlock
{
	public:
		WhileBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

	private:
		int defaultBodyHeight() const;

		int m_headerHeight = s_defaultHeaderHeight;

		Block* m_body{nullptr};
};

} // namespace Scratch

#endif // SCRATCHWHILEBLOCK_H
