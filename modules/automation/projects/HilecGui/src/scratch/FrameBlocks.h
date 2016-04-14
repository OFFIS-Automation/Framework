#ifndef FRAMEBLOCK_H
#define FRAMEBLOCK_H

#include "Block.h"

namespace Scratch
{

class FrameBlock : public Block
{
	protected:
		int s_midsegmentOffset = 30;
		int s_taperHeight = 20;

	public:
		FrameBlock();

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
};

class StartBlock : public FrameBlock
{
	public:
		StartBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);
};

class EndBlock : public FrameBlock
{
	public:
		EndBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);
};

} // namespace Scratch

#endif // FRAMEBLOCK_H
