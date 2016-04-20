#ifndef SCRATCHCONTROLFLOWBLOCK_H
#define SCRATCHCONTROLFLOWBLOCK_H

#include <ostream>
#include <memory>
#include <vector>

#include "Block.h"
#include "Condition.h"

namespace Scratch
{

class ControlFlowBlock : public Block
{
	protected:
		static const int s_shaftHeight = 30;
		static const int s_defaultHeaderHeight = s_shaftHeight;

		static const int s_conditionMargin = 10;

		constexpr int defaultConditionWidth() const
		{
			return m_defaultWidth - s_shaftHeight - 2 * s_conditionMargin;
		}

		constexpr int defaultConditionHeight() const
		{
			return s_defaultHeaderHeight - 2 * s_conditionMargin;
		}

	protected:
		ControlFlowBlock(const int width, const int height, const size_t numberOfBodies);

		virtual int defaultBodyHeight() const = 0;
		bool inConditionArea(const QPointF& position);

		void resizeBy(int dx, int dy, const QPointF& triggerPosition);

		void addCondition(Condition& condition);
		void addBody(Block& block, Block*& body, const QPoint &offset);

		int m_headerHeight = s_defaultHeaderHeight;

		Condition* m_condition{nullptr};

		std::vector<Block*> m_bodies;
};

} // namespace Scratch

#endif // SCRATCHCONTROLFLOWBLOCK_H
