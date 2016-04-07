#include "Condition.h"

namespace Scratch
{

std::ostream& operator<<(std::ostream& stream, const Scratch::Condition&)
{
	stream << "True"; // TODO

	return stream;
}

} // namespace Scratch
