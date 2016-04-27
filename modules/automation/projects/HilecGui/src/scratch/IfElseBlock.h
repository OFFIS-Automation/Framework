#ifndef SCRATCHIFELSEBLOCK_H
#define SCRATCHIFELSEBLOCK_H

#include "ControlFlowBlock.h"

namespace Scratch
{

class IfElseBlock : public ControlFlowBlock
{
	public:
		IfElseBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

	private:
		Body& m_trueBody;
		Body& m_falseBody;
};

} // namespace Scratch

#endif // SCRATCHIFELSEBLOCK_H
