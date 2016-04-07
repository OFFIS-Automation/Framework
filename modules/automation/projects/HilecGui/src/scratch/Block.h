#ifndef SCRATCHBLOCK_H
#define SCRATCHBLOCK_H

#include <ostream>
#include <functional>
#include <memory>

#include <QGraphicsItem>

namespace Scratch
{

class Block : public QGraphicsItem
{
	public:
		static const int s_connectorMargin = 10;
		static const int s_connectorHeight = 5;
		static const int s_connectorWidth = 20;
		static const int s_connectorMidsegmentWidth = 10;

		static const int s_connectorActivationRange = 10;

		virtual Block& clone() = 0;
		virtual void print(std::ostream& stream, unsigned indentationDepth) = 0;

		void setPredecessorsReference(Block** reference);
		void setSuccessor(Block* successor);
		void setParent(Block* parent);

		void updateSuccessorPositions(int dx, int dy);

		int m_width;
		int m_height;

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *) {}
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

		void resizeBy(int dx, int dy);

		void addBlock(Block& block);
		void addAbove(Block& block);
		void addBelow(Block& block);

		Block** m_predecessorsReference{nullptr};
		Block* m_successor{nullptr};
		Block* m_parent{nullptr};
};

} // namespace Scratch

#endif // SCRATCHBLOCK_H
