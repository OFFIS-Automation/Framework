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
	Block::paint(painter, item, widget);
	ArgumentItem::paint(painter, item, widget);
}

void ArgumentBlock::print(std::ostream& stream, unsigned indentationDepth) const
{
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	ArgumentItem::print(stream);

	stream << "\n";

	Block::print(stream, indentationDepth);
}

void ArgumentBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	if (Item::unpackItem(*event).itemType() == Item::Type::Block)
	{
		Block::dragMoveEvent(event);

		return;
	}

	ArgumentItem::dragMoveEvent(event);
}

void ArgumentBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	if (Item::unpackItem(*event).itemType() == Item::Type::Block)
	{
		Block::dropEvent(event);

		return;
	}

	ArgumentItem::dropEvent(event);
}

QPoint ArgumentBlock::resizeBy(int dx, int dy, const QPoint& triggerPosition)
{
	auto actualD = ArgumentItem::resizeBy(dx, dy, triggerPosition);
	actualD = Block::resizeBy(actualD.x(), actualD.y(), triggerPosition);

	return actualD;
}

} // namespace Scratch
