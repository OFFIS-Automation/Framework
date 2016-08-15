#include "VariableItems.h"

#include <QPainter>

namespace Scratch
{

SetVariableBlock::SetVariableBlock(const std::string& name, Item::Type type)
:	ArgumentBlock("set")
{
	addArgument(name, type);
}

Item& SetVariableBlock::clone() const
{
	return *(new SetVariableBlock(m_arguments.at(0).name, m_arguments.at(0).type));
}

void SetVariableBlock::print(std::ostream& stream, unsigned indentationDepth) const
{
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << m_arguments.at(0).name << " = " << m_arguments.at(0).parameter << std::endl;

	Block::print(stream, indentationDepth);
}

}
