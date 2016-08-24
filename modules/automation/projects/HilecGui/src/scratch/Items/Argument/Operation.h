#ifndef OPERATION_H
#define OPERATION_H

#include "ArgumentItem.h"

namespace Scratch
{

template <typename T>
class Operation : public Argument<T>
{
	public:
		Operation(const std::string& name, Item::Type type, bool createArguments = true);
		Operation(const std::string& name, Item::Type lhs, Item::Type rhs,
			bool createArguments = true);

		Item& clone() const;

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);
		void print(std::ostream& stream) const;

	protected:
		bool updateItem();

	private:
		Item::Type m_lhs, m_rhs;
};

template <typename T>
Operation<T>::Operation(const std::string& name, Item::Type type, bool createArguments)
:	Operation<T>(name, type, type, createArguments)
{}

template <typename T>
Operation<T>::Operation(const std::string& name, Item::Type lhs, Item::Type rhs,
	bool createArguments)
:	Argument<T>(name),
	m_lhs(lhs),
	m_rhs(rhs)
{
	if (createArguments)
	{
		addArgument("", m_lhs);
		addArgument("", m_rhs);
	}
}

template <typename T>
Item& Operation<T>::clone() const
{
	auto& operation = *(new Operation<T>(m_name, m_lhs, m_rhs, false));

	operation.addArgument("", m_lhs, true);
	operation.addArgument("", m_rhs, true);

	return operation;
}

template <typename T>
void Operation<T>::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
	const QFontMetrics fontMetric(m_font);

	T::paint(painter, item, widget);

	const auto& firstParameter = m_arguments.at(0).getParameter();

	const auto position = QPointF(
		firstParameter.pos().x() + firstParameter.m_width + s_margin / 2,
		m_height / 2 + fontMetric.height() / 2);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(position, m_name.c_str());
}

template <typename T>
void Operation<T>::print(std::ostream& stream) const
{
	stream << m_arguments.at(0).getParameter() << " " << m_name << " "
		<< m_arguments.at(1).getParameter();
}

template <typename T>
bool Operation<T>::updateItem()
{
	if (m_arguments.size() < 2)
		return false;

	QFontMetrics fontMetric(m_font);

	prepareGeometryChange();

	auto updated = ArgumentItem::updateItem(); // -> Height
	updated |= T::updateItem(); // -> Margin
	updated |= ArgumentItem::updateItem(); // -> Height

	// Position arguments

	auto position = m_horizontalMargin;

	auto setPosition = [&](auto& argument)
	{
		auto& paramter = argument.getParameter();

		paramter.setPos(position, (m_height - paramter.m_height) / 2);

		position += paramter.m_width;
	};

	setPosition(m_arguments.at(0));
	position += fontMetric.width(QString::fromStdString(m_name)) + s_margin;
	setPosition(m_arguments.at(1));

	// Set extends

	const auto newWidth = position + m_horizontalMargin;

	if (!updated && newWidth == m_width)
		return false;

	m_width = newWidth;

	if (m_parent)
		m_parent->updateItem();

	return true;
}

}

#endif // OPERATION_H
