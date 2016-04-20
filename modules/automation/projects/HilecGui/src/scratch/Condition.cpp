#include "Condition.h"

namespace Scratch
{

Condition::Condition(int width, int height)
	: Item(width, height)
{}

QRectF Condition::boundingRect() const
{
	return QRect(0, 0, m_width, m_height);
}

Item::Type Condition::itemType() const
{
	return Type::Condition;
}

void Condition::remove()
{
	if (m_parent)
		m_parent->resizeBy(-m_width, -m_height, pos());

	Item::remove();
}

std::ostream& operator<<(std::ostream& stream, const Scratch::Condition& condition)
{
	condition.print(stream);

	return stream;
}

} // namespace Scratch
