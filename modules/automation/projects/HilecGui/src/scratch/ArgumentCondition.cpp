#include "ArgumentCondition.h"

namespace Scratch
{

ArgumentCondition::ArgumentCondition(const std::string& name)
:	Item(120, 40),
	ArgumentItem(name)
{
	updateItem();
}

Item& ArgumentCondition::clone() const
{
	auto& argumentNumber = *(new ArgumentCondition(m_name));

	for (const auto& argument : m_arguments)
		argumentNumber.addArgument(argument.name, argument.type, true);

	return argumentNumber;
}

void ArgumentCondition::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
	Condition::paint(painter, item, widget);
	ArgumentItem::paint(painter, item, widget);
}

void ArgumentCondition::print(std::ostream& stream) const
{
	ArgumentItem::print(stream);
}

void ArgumentCondition::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	ArgumentItem::dragMoveEvent(event);
}

void ArgumentCondition::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	ArgumentItem::dropEvent(event);
}

bool ArgumentCondition::updateItem()
{
	auto updated = ArgumentItem::updateItem(); // -> Height
	updated |= Condition::updateItem(); // -> Margin
	updated |= ArgumentItem::updateItem(); // -> Width

	return updated;
}

} // namespace Scratch
