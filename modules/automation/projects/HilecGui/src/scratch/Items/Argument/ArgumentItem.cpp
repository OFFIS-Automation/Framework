#include "ArgumentItem.h"

#include <algorithm>

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsScene>

#include "../Parameter/Default/DefaultCondition.h"
#include "../Parameter/Default/DefaultNumber.h"
#include "../Parameter/Default/DefaultPoint.h"

namespace Scratch
{

ArgumentItem::ArgumentItem(const std::string& name)
:	m_name{name}
{
	m_width = QFontMetrics(m_font).width(QString::fromStdString(name)) + 2 * m_horizontalMargin;

	updateItem();
}

void ArgumentItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QFontMetrics fontMetric(m_font);

	auto x = m_horizontalMargin;

	auto paintName = [&](auto&& name)
	{
		auto position = QPointF(x, m_height / 2 + fontMetric.height() / 4);

		painter->setPen(m_textStyle);
		painter->setFont(m_font);
		painter->drawText(position, name);

		x += fontMetric.width(name);
	};

	paintName(QString::fromStdString(m_name));
	x += s_margin;

	for (const auto& argument : m_arguments)
	{
		paintName(QString::fromStdString(argument.name + ":"));
		x += s_margin / 2;

		x += argument.getParameter().m_width;

		x += s_margin;
	}
}

void ArgumentItem::print(std::ostream& stream) const
{
	stream << m_name << "(";

	for (auto argument = m_arguments.cbegin(); argument != m_arguments.cend(); ++argument)
	{
		stream << argument->getParameter();

		if (argument + 1 != m_arguments.cend())
			stream << ", ";
	}

	stream << ")";
}

ArgumentItem::Argument& ArgumentItem::addArgument(const std::string& name, const Item::Type& type,
	const bool enable)
{
	Parameter* defaultParameter;

	if (type == Item::Type::Number)
		defaultParameter = reinterpret_cast<Parameter*>(new DefaultNumber(enable));
	else if (type == Item::Type::Condition)
		defaultParameter = reinterpret_cast<Parameter*>(new DefaultCondition(enable));
	else if (type == Item::Type::Point)
		defaultParameter = reinterpret_cast<Parameter*>(new DefaultPoint(enable));

	defaultParameter->setParent(this);

	m_arguments.push_back({name, type, *defaultParameter, nullptr});

	updateItem();

	return m_arguments.back();
}

void ArgumentItem::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos().toPoint();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() == Item::Type::Block)
		return;

	Parameter& parameter = *dynamic_cast<Parameter*>(&item);

	const auto argument =
		std::find_if(m_arguments.cbegin(), m_arguments.cend(), [&](const Argument& argument)
			{
				if (argument.parameter)
					return false;

				if (parameter.itemType() != argument.type)
					return false;

				const auto parameterPosition = argument.defaultParameter.mapFromParent(position);

				return argument.defaultParameter.contains(parameterPosition);
			});

	if (argument == m_arguments.cend())
		return;

	if (item.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);
}

void ArgumentItem::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	auto* item = &Item::unpackItem(*event);

	auto handleEvent = [&]()
	{
		event->accept();

		// Copy
		if (item->scene() != this->scene())
		{
			event->setDropAction(Qt::CopyAction);

			item = &item->clone();
		}
		// Move
		else
		{
			event->setDropAction(Qt::MoveAction);
			item->remove();
		}
	};

	for (auto& argument : m_arguments)
	{
		const auto parameterPosition = argument.defaultParameter.mapFromParent(
			position.toPoint());

		if (argument.defaultParameter.contains(parameterPosition))
		{
			handleEvent();

			auto& parameter = *dynamic_cast<Parameter*>(item);

			addParameter(parameter, argument);

			return;
		}
	}
}

bool ArgumentItem::updateItem()
{
	QFontMetrics fontMetric(m_font);

	prepareGeometryChange();

	// Find highest argument

	auto highestArgument = defaultHeight() - 2 * s_margin;

	for (auto& argument : m_arguments)
	{
		const auto& paramter = argument.getParameter();

		if (highestArgument < paramter.m_height)
			highestArgument = paramter.m_height;
	}

	// Position arguments

	auto position = m_horizontalMargin + fontMetric.width(QString::fromStdString(m_name));

	for (auto& argument : m_arguments)
	{
		argument.defaultParameter.setVisible(!argument.parameter);

		auto& paramter = argument.getParameter();

		const auto appendageWidth = s_margin + (argument.name != ""?
			fontMetric.width(QString::fromStdString(argument.name + ":")) + s_margin / 2 : 0);

		position += appendageWidth;

		paramter.setPos(position, s_margin + (highestArgument - paramter.m_height) / 2);

		position += paramter.m_width;
	}

	// Set extends

	const auto newWidth = position + m_horizontalMargin;
	const auto newHeight = 2 * s_margin + highestArgument;

	if (newWidth == m_width && newHeight == m_height)
		return false;

	m_width = newWidth;
	m_height = newHeight;

	if (m_parent)
		m_parent->updateItem();

	return true;
}

void ArgumentItem::addParameter(Parameter& parameter, Argument& argument)
{
	assert(!argument.parameter && "Parameter of the argument is already assigned");

	parameter.setParent(this);
	parameter.m_parentReference = reinterpret_cast<Parameter**>(&argument.parameter);

	argument.parameter = &parameter;

	updateItem();
}

} // namespace Scratch
