#include "Condition.h"

namespace Scratch
{

std::ostream& operator<<(std::ostream& stream, const Scratch::Condition& other)
{
	static_cast<void>(other);

	stream << "True"; // TODO

	return stream;
}

} // namespace Scratch
