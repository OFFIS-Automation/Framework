#ifndef SCRATCHCONDITION_H
#define SCRATCHCONDITION_H

#include <ostream>

namespace Scratch
{

class Condition
{
	friend std::ostream& operator<<(std::ostream&, const Condition&);
};

std::ostream& operator<<(std::ostream&, const Scratch::Condition&);

} // namespace Scratch

#endif // SCRATCHCONDITION_H


