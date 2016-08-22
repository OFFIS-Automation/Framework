#ifndef SCRATCHWHILEBLOCK_H
#define SCRATCHWHILEBLOCK_H

#include "ControlFlowBlock.h"

namespace Scratch
{

class WhileBlock : public ControlFlowBlock
{
	public:
		WhileBlock(bool enable = false);

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

	private:
		Body& m_body;
};

} // namespace Scratch

#endif // SCRATCHWHILEBLOCK_H
