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

		static bool inConnectorActivationRange(const QPointF& position, const int& offset);

		static Block& unpackBlock(const QGraphicsSceneDragDropEvent& event);

	public:
		QRectF boundingRect() const;

		virtual Block& clone() const = 0;
		virtual void print(std::ostream& stream, unsigned indentationDepth) const = 0;

		void setPredecessorsReference(Block** reference);
		void setSuccessor(Block* successor);
		void setParent(Block* parent);

		void remove();
		void updateSuccessorPositions(int dx, int dy) const;

		int m_width;
		int m_height;

	protected:
		const int m_defaultWidth;
		const int m_defaultHeight;

		Block(const int defaultWidth, const int defaultHeight);

		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

		void addConnector(QPolygon& polygon, int x, int y, bool reverse = false) const;
		virtual void resizeBy(int dx, int dy, const QPointF&);

		void addBlock(Block& block);
		void addAbove(Block& block);
		void addBelow(Block& block);

		bool isSelfOrAncestor(Block &block);

		Block** m_predecessorsReference{nullptr};
		Block* m_successor{nullptr};
		Block* m_parent{nullptr};
};

} // namespace Scratch

#endif // SCRATCHBLOCK_H
