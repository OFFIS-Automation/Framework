#include <stdexcept>
#include <ostream>

#include "IfElseBlock.h"

namespace Scratch
{

void IfElseBlock::print(std::ostream& stream, unsigned indentationDepth = 0)
{
	if (!m_condition)
		throw std::invalid_argument("Not all conditions are defined.");

	if (!m_ifBody || !m_elseBody)
		throw std::invalid_argument("Not all if bodies are defined.");

	// if

	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "if " << *m_condition << ":\n";

	m_ifBody->print(stream, indentationDepth + 1);

	// else

	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "else:\n";

	m_elseBody->print(stream, indentationDepth + 1);

	// next

	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

} // namespace Scratch
