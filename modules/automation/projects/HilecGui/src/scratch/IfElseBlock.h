#ifndef SCRATCHIFELSEBLOCK_H
#define SCRATCHIFELSEBLOCK_H

#include "ControlFlowBlock.h"

namespace Scratch
{

class IfElseBlock : public ControlFlowBlock
{
	public:
		IfElseBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget*widget);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		void resizeBy(int dx, int dy, const QPointF& triggerPosition);

	private:
		int defaultBodyHeight() const;

		int m_trueBodyHeight = defaultBodyHeight();

		Block*& m_trueBody;
		Block*& m_falseBody;
};

} // namespace Scratch

#endif // SCRATCHIFELSEBLOCK_H
