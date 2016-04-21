#ifndef SCRATCHIFELSEBLOCK_H
#define SCRATCHIFELSEBLOCK_H

#include "ControlFlowBlock.h"

namespace Scratch
{

class IfElseBlock : public ControlFlowBlock<2>
{
	public:
		IfElseBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

	private:
		int m_trueBodyHeight = defaultBodyHeight();

		Body& m_trueBody;
		Body& m_falseBody;
};

} // namespace Scratch

#endif // SCRATCHIFELSEBLOCK_H
