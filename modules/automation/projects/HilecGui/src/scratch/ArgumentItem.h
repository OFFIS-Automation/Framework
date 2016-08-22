#ifndef PARAMETERBLOCK_H
#define PARAMETERBLOCK_H

#include <string>

#include <QPainter>

#include "Block.h"
#include "Parameter.h"

namespace Scratch
{

class ArgumentItem : virtual public Item
{
	protected:
		struct Argument
		{
			std::string name;
			Item::Type type;

			Parameter& defaultParameter;
			Parameter* parameter;

			Parameter& getParameter() const
			{
				return parameter? *parameter : defaultParameter;
			}
		};

	public:
		ArgumentItem(const std::string& name);

		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		virtual void print(std::ostream& stream) const;

		Argument& addArgument(const std::string& name, const Item::Type& type,
			const bool enable = false);
		void addParameter(Parameter& parameter, Argument& argument);

	protected:
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		virtual void dropEvent(QGraphicsSceneDragDropEvent* event);

		virtual bool updateItem();

		std::string m_name;
		std::vector<Argument> m_arguments;
};

// Generic argument item

template <typename T>
class Argument : public ArgumentItem, public T
{
	public:
		Argument(const std::string& name);

		Item& clone() const;

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);
		void print(std::ostream& stream) const;

	protected:
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		virtual void dropEvent(QGraphicsSceneDragDropEvent* event);

		virtual bool updateItem();
};

template <typename T>
Argument<T>::Argument(const std::string& name)
:	ArgumentItem(name)
{}

template <typename T>
Item& Argument<T>::clone() const
{
	auto& argumentNumber = *(new Argument(m_name));

	for (const auto& argument : m_arguments)
		argumentNumber.addArgument(argument.name, argument.type, true);

	return argumentNumber;
}

template <typename T>
void Argument<T>::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
	T::paint(painter, item, widget);
	ArgumentItem::paint(painter, item, widget);
}

template <typename T>
void Argument<T>::print(std::ostream& stream) const
{
	ArgumentItem::print(stream);
}

template <typename T>
void Argument<T>::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	ArgumentItem::dragMoveEvent(event);
}

template <typename T>
void Argument<T>::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	ArgumentItem::dropEvent(event);
}

template <typename T>
bool Argument<T>::updateItem()
{
	auto updated = ArgumentItem::updateItem(); // -> Height
	updated |= T::updateItem(); // -> Margin
	updated |= ArgumentItem::updateItem(); // -> Width

	return updated;
}

} // namespace Scratch

#endif // PARAMETERBLOCK_H
