#include "ArgumentBlock.h"

namespace Scratch
{

ArgumentBlock::ArgumentBlock(const std::string& name)
	: Item(s_defaultWidth, s_defaultHeight),
	  ArgumentItem(name)
{}

Item& ArgumentBlock::clone() const
{
	auto& argumentBlock =  *(new ArgumentBlock(m_name));

	for (const auto& argument : m_arguments)
		argumentBlock.addArgument(argument.name, argument.type, true);

	return argumentBlock;
}

void ArgumentBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
	QPolygon polygon;
	drawOutline(polygon);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

	ArgumentItem::paint(painter, item, widget);
}

void ArgumentBlock::print(std::ostream& stream, unsigned indentationDepth) const
{
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	ArgumentItem::print(stream);
}

void ArgumentBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	Block::dragMoveEvent(event);

	if (Item::unpackItem(*event).itemType() == Item::Type::Block)
		return;

	ArgumentItem::dragMoveEvent(event);
}

void ArgumentBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	Block::dropEvent(event);
	ArgumentItem::dropEvent(event);
}

QPoint ArgumentBlock::resizeBy(int dx, int dy, const QPoint& triggerPosition)
{
	auto actualD = ArgumentItem::resizeBy(dx, dy, triggerPosition);
	actualD = Block::resizeBy(actualD.x(), actualD.y(), triggerPosition);

	return actualD;
}

} // namespace Scratch
