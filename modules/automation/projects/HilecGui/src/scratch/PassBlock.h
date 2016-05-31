#ifndef PASSBLOCK_H
#define PASSBLOCK_H

#include "Block.h"

namespace Scratch
{

class PassBlock : public Block
{
	public:
		PassBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*item, QWidget*widget);

		Block& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;
};

} // namespace Scratch

#endif // PASSBLOCK_H
