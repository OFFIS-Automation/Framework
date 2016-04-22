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
		static const int s_margin = 15;

		struct Argument
		{
			std::string name;
			Item::Type type;
			Parameter* parameter;
		};

	public:
		ArgumentBlock();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Item& clone() const;
		void print(std::ostream& stream, unsigned indentationDepth) const;

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		void resizeBy(int dx, int dy, const QPointF& triggerPosition);

		void addParameter(Parameter& parameter);

		std::string m_name;
		std::vector<Argument> m_arguments;
};

} // namespace Scratch

#endif // PARAMETERBLOCK_H
