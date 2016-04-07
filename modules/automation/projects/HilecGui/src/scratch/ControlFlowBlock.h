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
	public:
		static const unsigned s_shaftHeight = 30;

	protected:
		Condition* m_condition = nullptr;
};

} // namespace Scratch

#endif // SCRATCHCONTROLFLOWBLOCK_H
