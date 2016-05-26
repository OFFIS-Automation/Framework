#ifndef SCRATCHCONTROLFLOWBLOCK_H
#define SCRATCHCONTROLFLOWBLOCK_H

#include <ostream>
#include <memory>
#include <vector>
#include <algorithm>

#include "Block.h"
#include "ArgumentItem.h"

namespace Scratch
{

class ControlFlowBlock : public Block, public ArgumentItem
{
	protected:
		static const int s_shaftExtent = 40;
		static const int s_defaultHeaderHeight = s_shaftExtent;

		int defaultConditionWidth() const
		{
			return m_defaultWidth - s_shaftExtent - 2 * s_margin;
		}

		int defaultConditionHeight() const
		{
			return s_defaultHeaderHeight - 2 * s_margin;
		}

		int defaultBodyHeight() const
		{
			return (m_defaultHeight - (m_bodies.size() + 1) * s_defaultHeaderHeight)
				/ m_bodies.size();
		}

		struct Body
		{
			int height;
			Block* block;
		};

	public:
		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		QPoint resizeBy(int dx, int dy, const QPoint& triggerPosition);

		void addArgument(const std::string& name, const Item::Type& type,
			const bool enable = false);

	protected:
		ControlFlowBlock(const std::string& name, const size_t& numberOfBodies);

		bool inBodyRange(const QPoint& position);

		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		void addBody(Block& block, Block*& bodyBlock, const QPoint &offset);

		int m_headerHeight = s_defaultHeaderHeight;
		std::vector<Body> m_bodies{};
};

} // namespace Scratch

#endif // SCRATCHCONTROLFLOWBLOCK_H
