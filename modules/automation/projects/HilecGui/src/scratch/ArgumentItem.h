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

} // namespace Scratch

#endif // PARAMETERBLOCK_H
