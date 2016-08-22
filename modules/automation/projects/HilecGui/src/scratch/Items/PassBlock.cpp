#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include "PassBlock.h"

namespace Scratch
{

void PassBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
	Block::paint(painter, item, widget);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(boundingRect(), Qt::AlignCenter, "Pass");
}

Block& PassBlock::clone() const
{
	return *(new PassBlock());
}

void PassBlock::print(std::ostream& stream, unsigned indentationDepth = 0) const
{
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "pass" << std::endl;

	Block::print(stream, indentationDepth);
}

} // namespace Scratch
