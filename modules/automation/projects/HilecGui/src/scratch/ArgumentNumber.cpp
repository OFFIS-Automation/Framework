#include "ArgumentNumber.h"

namespace Scratch
{

ArgumentNumber::ArgumentNumber(const std::string& name)
:	ArgumentItem(name)
{}

Item& ArgumentNumber::clone() const
{
	auto& argumentNumber = *(new ArgumentNumber(m_name));

	for (const auto& argument : m_arguments)
		argumentNumber.addArgument(argument.name, argument.type, true);

	return argumentNumber;
}

void ArgumentNumber::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
	Number::paint(painter, item, widget);
	ArgumentItem::paint(painter, item, widget);
}

void ArgumentNumber::print(std::ostream& stream) const
{
	ArgumentItem::print(stream);
}

void ArgumentNumber::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	ArgumentItem::dragMoveEvent(event);
}

void ArgumentNumber::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	ArgumentItem::dropEvent(event);
}

bool ArgumentNumber::updateItem()
{
	auto updated = ArgumentItem::updateItem(); // -> Height
	updated |= Number::updateItem(); // -> Margin
	updated |= ArgumentItem::updateItem(); // -> Width

	return updated;
}

} // namespace Scratch
