#ifndef SCRATCHCONTROLFLOWBLOCK_H
#define SCRATCHCONTROLFLOWBLOCK_H

#include <ostream>
#include <memory>

#include "Block.h"
#include "Condition.h"

namespace Scratch
{

class ControlFlowBlock : public Block
{
	protected:
		static const unsigned s_shaftHeight = 30;
		static const int s_defaultHeaderHeight = s_shaftHeight;

	protected:
		ControlFlowBlock(const int defaultWidth, const int defaultHeight)
			: Block(defaultWidth, defaultHeight)
		{}

		virtual int defaultBodyHeight() const = 0;
		void addBody(Block& block, Block*& body, const QPoint &offset);

		Condition* m_condition{nullptr};
};

} // namespace Scratch

#endif // SCRATCHCONTROLFLOWBLOCK_H
