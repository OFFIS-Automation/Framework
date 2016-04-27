#ifndef SCRATCHCONTROLFLOWBLOCK_H
#define SCRATCHCONTROLFLOWBLOCK_H

#include <ostream>
#include <memory>
#include <vector>
#include <algorithm>

#include "Block.h"
#include "Condition.h"

namespace Scratch
{

class ControlFlowBlock : public Block
{
	protected:
		static const int s_shaftExtent = 40;
		static const int s_defaultHeaderHeight = s_shaftExtent;

		constexpr int defaultConditionWidth() const
		{
			return m_defaultWidth - s_shaftExtent - 2 * s_margin;
		}

		constexpr int defaultConditionHeight() const
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

		void resizeBy(int dx, int dy, const QPoint& triggerPosition);

	protected:
		ControlFlowBlock(const int width, const int height, const size_t& numberOfBodies);

		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		QRect conditionArea();

		void addCondition(Condition& condition);
		void addBody(Block& block, Block*& bodyBlock, const QPoint &offset);

		int m_headerHeight = s_defaultHeaderHeight;

		Condition* m_condition{nullptr};

		std::vector<Body> m_bodies{};
};

} // namespace Scratch

#endif // SCRATCHCONTROLFLOWBLOCK_H
