#include "ArgumentBlock.h"

namespace Scratch
{

Argument<Block>::Argument(const std::string& name)
:	ArgumentItem(name)
{
	updateItem();
}

Item& Argument<Block>::clone() const
{
	auto& argumentBlock =  *(new Argument<Block>(m_name));

	for (const auto& argument : m_arguments)
		argumentBlock.addArgument(argument.name, argument.type, true);

	return argumentBlock;
}

void Argument<Block>::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
	Block::paint(painter, item, widget);
	ArgumentItem::paint(painter, item, widget);
}

void Argument<Block>::print(std::ostream& stream, unsigned indentationDepth) const
{
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	ArgumentItem::print(stream);

	stream << std::endl;

	Block::print(stream, indentationDepth);
}

void Argument<Block>::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	if (Item::unpackItem(*event).itemType() == Item::Type::Block)
	{
		Block::dragMoveEvent(event);

		return;
	}

	ArgumentItem::dragMoveEvent(event);
}

void Argument<Block>::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	if (Item::unpackItem(*event).itemType() == Item::Type::Block)
		Block::dropEvent(event);
	else
		ArgumentItem::dropEvent(event);
}

bool Argument<Block>::updateItem()
{
	auto updated = ArgumentItem::updateItem();
	updated |= Block::updateItem();

	return updated;
}

} // namespace Scratch
