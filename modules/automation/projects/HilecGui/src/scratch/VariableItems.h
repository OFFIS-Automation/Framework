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
		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		Item& clone() const;
		void print(std::ostream& stream) const;

	private:
		std::string m_name;
};

template <typename ParameterType>
VariableParameter<ParameterType>::VariableParameter(const std::string& name)
:	m_name(name)
{}

template <typename ParameterType>
void VariableParameter<ParameterType>::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon outline;

	drawOutline(outline);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(outline);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(boundingRect(), Qt::AlignCenter, QString(m_name.c_str()));
}

template <typename ParameterType>
Item& VariableParameter<ParameterType>::clone() const
{
	return *(new VariableParameter(m_name));
}

template <typename ParameterType>
void VariableParameter<ParameterType>::print(std::ostream& stream) const
{
	stream << m_name;
}

}

#endif // VARIABLEITEMS_H
