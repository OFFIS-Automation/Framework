#ifndef SCRATCHIFELSEBLOCK_H
#define SCRATCHIFELSEBLOCK_H

#include "ControlFlowBlock.h"

namespace Scratch
{

class IfElseBlock : public ControlFlowBlock
{
	public:
		void print(std::ostream& stream, unsigned indentationDepth);

	private:
		std::unique_ptr<Condition> m_condition;
		std::unique_ptr<Block> m_ifBody, m_elseBody;
};

} // namespace Scratch

#endif // SCRATCHIFELSEBLOCK_H
