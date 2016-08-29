#ifndef FRAMEBLOCK_H
#define FRAMEBLOCK_H

#include "Block.h"
#include "Argument/ArgumentBlock.h"

namespace Scratch
{

class FrameBlock : public Block, public ArgumentItem
{
	public:
		FrameBlock(const std::string& name);

		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) = 0;

		bool updateItem();

	protected:
		int s_midsegmentOffset = 30;
		int s_taperHeight = 20;

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

		void dragMoveEvent(QGraphicsSceneDragDropEvent* event) = 0;
		void dropEvent(QGraphicsSceneDragDropEvent* event) = 0;
};

class StartBlock : public FrameBlock
{
	public:
		StartBlock(const std::string& name);

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth = 0) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);
};

class EndBlock : public FrameBlock
{
	public:
		EndBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

		Block& clone() const;
		void print(std::ostream&stream, unsigned indentationDepth = 0) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);
};

} // namespace Scratch

#endif // FRAMEBLOCK_H
