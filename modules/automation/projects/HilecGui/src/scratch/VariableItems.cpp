#include "VariableItems.h"

#include <QPainter>

namespace Scratch
{

SetVariableBlock::SetVariableBlock(const std::string& name, Item::Type type, bool enable)
:	ArgumentBlock("set")
{
	addArgument(name, type, enable);
}

Item& SetVariableBlock::clone() const
{
	return *(new SetVariableBlock(m_arguments.at(0).name, m_arguments.at(0).type, true));
}

void SetVariableBlock::print(std::ostream& stream, unsigned indentationDepth) const
{
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << m_arguments.at(0).name << " = " << m_arguments.at(0).getParameter() << std::endl;

	Block::print(stream, indentationDepth);
}

}
