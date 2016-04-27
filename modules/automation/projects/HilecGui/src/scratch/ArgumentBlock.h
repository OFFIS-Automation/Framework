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
		static const int s_defaultParameterWidth = 40;

		constexpr int parameterHeight() const
		{
			return m_height - 2 * s_margin;
		}

		struct Argument
		{
			std::string name;
			Item::Type type;

			int x;

			Parameter* parameter;
		};

	public:
		ArgumentBlock(const std::string& name);

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Item& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

		void addArgument(const std::string& name, const Item::Type& type);

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		void resizeBy(int dx, int dy, const QPoint& triggerPosition);

		void addParameter(Parameter& parameter, Parameter*& argumentParameter, const QPoint &offset);

		std::string m_name;
		std::vector<Argument> m_arguments;
};

} // namespace Scratch

#endif // PARAMETERBLOCK_H
