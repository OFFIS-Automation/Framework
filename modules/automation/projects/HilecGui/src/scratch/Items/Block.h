#ifndef SCRATCHBLOCK_H
#define SCRATCHBLOCK_H

#include <ostream>
#include <functional>
#include <memory>

#include <QPen>

#include "Item.h"

namespace Scratch
{

class Block : public virtual Item
{
	public:
		static const int s_connectorHeight = 5;
		static const int s_connectorWidth = 4 * s_connectorHeight;
		static const int s_connectorMidsegmentOffset = s_connectorHeight;

		static const int s_connectorActivationRange = 2 * s_connectorHeight;

		static bool inConnectorActivationRange(const QPointF& position, const int& offset);

	public:
		Type itemType() const final;

		virtual QRectF boundingRect() const final;

		virtual void print(std::ostream& stream, unsigned indentationDepth) const;

		void setPredecessorsReference(Block** reference);
		void setSuccessor(Block* successor);

		void addAbove(Block& block);
		void addBelow(Block& block);
		void remove();

		virtual int defaultHeight();
		virtual bool updateItem();

		void updateSuccessorPositions(int dy) const;

		Block** m_predecessorsReference{nullptr};
		Block* m_successor{nullptr};

	protected:
		Block();

		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		void drawConnector(QPolygon& polygon, int x, int y, bool reverse = false) const;
		void drawOutline(QPolygon& polygon) const;

		bool isSelfOrAncestor(Block &block);

		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		virtual void dropEvent(QGraphicsSceneDragDropEvent* event);
};

} // namespace Scratch

#endif // SCRATCHBLOCK_H
