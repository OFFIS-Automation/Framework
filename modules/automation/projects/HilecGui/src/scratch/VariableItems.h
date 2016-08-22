#ifndef VARIABLEITEMS_H
#define VARIABLEITEMS_H

#include "ArgumentBlock.h"
#include "Condition.h"
#include "Number.h"

namespace Scratch
{

class SetVariableBlock : public ArgumentBlock
{
	public:
		SetVariableBlock(const std::string& name, Item::Type type, bool enable = false);

		Item& clone() const;

		void print(std::ostream& stream, unsigned indentationDepth) const;
};

template <typename ParameterType>
class VariableParameter : public ParameterType
{
	public:
		VariableParameter(const std::string& name);

		Item& clone() const;

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
		void print(std::ostream& stream) const;

	protected:
		bool updateItem();

	private:
		std::string m_name;
};

template <typename ParameterType>
VariableParameter<ParameterType>::VariableParameter(const std::string& name)
:	m_name(name)
{
	updateItem();
}

template <typename ParameterType>
void VariableParameter<ParameterType>::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPainterPath outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPath(outline);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(boundingRect(), Qt::AlignCenter, QString(m_name.c_str()));
}

template <typename ParameterType>
Item& VariableParameter<ParameterType>::clone() const
{
	return *(new VariableParameter<ParameterType>(m_name));
}

template <typename ParameterType>
void VariableParameter<ParameterType>::print(std::ostream& stream) const
{
	stream << m_name;
}

template <typename ParameterType>
bool VariableParameter<ParameterType>::updateItem()
{
	auto oldWidth = m_width;

	auto updated = Item::updateItem(); // -> height
	updated |= ParameterType::updateItem(); // -> margin

	m_width = QFontMetrics(m_font).width(m_name.c_str()) + 2 * m_horizontalMargin;

	return updated || oldWidth != m_width;
}

}

#endif // VARIABLEITEMS_H
