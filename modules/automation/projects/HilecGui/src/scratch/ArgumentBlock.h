#ifndef PARAMETERBLOCK_H
#define PARAMETERBLOCK_H

#include <string>

#include "Block.h"
#include "Parameter.h"

namespace Scratch
{

class ArgumentBlock : public Block
{
	protected:
		struct Argument
		{
			std::string name;
			Item::Type type;

			Parameter& defaultParameter;
			Parameter* parameter;
		};

	public:
		ArgumentBlock(const std::string& name);

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Item& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

		void addArgument(const std::string& name, const Item::Type& type, const bool enable = false);

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		void resizeBy(int dx, int dy, const QPoint& triggerPosition);

		void addParameter(Parameter& parameter, Argument& argument, const QPoint &offset);

		std::string m_name;
		std::vector<Argument> m_arguments;
};

} // namespace Scratch

#endif // PARAMETERBLOCK_H
