#include "ArgumentItem.h"

#include <algorithm>

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsScene>

#include "DefaultCondition.h"
#include "DefaultNumber.h"

namespace Scratch
{

ArgumentItem::ArgumentItem(const std::string& name)
	: m_name{name}
{
	m_width = QFontMetrics(m_font).width(QString::fromStdString(name)) + 2 * s_margin;

	setAcceptDrops(true);
}

void ArgumentItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QFontMetrics fontMetric(m_font);

	auto x = s_margin;

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

		if (argument.parameter)
			x += argument.parameter->m_width + s_margin;
		else
			x += argument.defaultParameter.m_width + s_margin;
	}
}

void ArgumentItem::print(std::ostream& stream) const
{
	stream << m_name << "(";

	for (auto argument = m_arguments.cbegin(); argument != m_arguments.cend(); ++argument)
	{
		if (argument->parameter)
			stream << *argument->parameter;
		else
			stream << argument->defaultParameter;

		if (argument + 1 != m_arguments.cend())
			stream << ", ";
	}

	stream << ")\n";
}

void ArgumentItem::addArgument(const std::string& name, const Item::Type& type, const bool enable)
{
	QFontMetrics fontMetric(m_font);

	const auto appendageWidth =
		s_margin + fontMetric.width(QString::fromStdString(name + ":")) + s_margin / 2;

	const auto x = (m_arguments.empty()?
			s_margin + fontMetric.width(QString::fromStdString(m_name)) :
			m_arguments.back().defaultParameter.pos().x() + (m_arguments.back().parameter?
				m_arguments.back().parameter->m_width :
				m_arguments.back().defaultParameter.m_width))
		 + appendageWidth;

	m_width += appendageWidth;

	auto& defaultParameter = *(type == Item::Type::Number?
		reinterpret_cast<Parameter*>(new DefaultNumber(enable))
		: reinterpret_cast<Parameter*>(new DefaultCondition(enable)));

	defaultParameter.setParent(this);
	defaultParameter.setPos(x, s_margin);

	m_width += defaultParameter.m_width;

	auto height = defaultParameter.m_height + 2 * s_margin;
	m_height = std::max(height, m_height);

	m_arguments.push_back({name, type, defaultParameter, nullptr});
}

void ArgumentItem::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() == Item::Type::Block)
		return;

	Parameter& parameter = *reinterpret_cast<Parameter*>(&item);

	const auto argument =
		std::find_if(m_arguments.cbegin(), m_arguments.cend(), [&](const Argument& argument)
			{
				if (argument.parameter)
					return false;

				if (parameter.itemType() != argument.type)
					return false;

				const auto parameterPosition = argument.defaultParameter.mapFromParent(
					position.toPoint());

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
	event->accept();

	const auto& position = event->pos();

	const auto oldHeight = m_height;
	const auto oldArguments = m_arguments;

	auto* item = &Item::unpackItem(*event);

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

	auto& parameter = *reinterpret_cast<Parameter*>(item);

	auto oldArgument = oldArguments.cbegin();

	for (auto &argument : m_arguments)
	{
		const auto parameterPosition = argument.defaultParameter.mapFromParent(
			position.toPoint());

		if (oldArgument->defaultParameter.contains(parameterPosition))
		{
			const auto position = QPoint(argument.defaultParameter.x(), s_margin);

			addParameter(parameter, argument, position);

			return;
		}

		++oldArgument;
	}
}

QPoint ArgumentItem::resizeBy(int dx, int dy, const QPoint& triggerPosition)
{
	auto actualDx = dx;
	auto actualDy = dy;

	auto moveParameters = [&](auto&& start, auto&& dx)
	{
		std::for_each(start, m_arguments.end(), [&](auto& argument)
		{
			argument.defaultParameter.moveBy(dx, 0);

			if (argument.parameter)
				argument.parameter->moveBy(dx, 0);
		});
	};

	for (auto argument = m_arguments.begin(); argument != m_arguments.end(); ++argument)
	{
		const auto& boundingRectangle =
			argument->parameter?
				argument->parameter->boundingRect().toRect() :
				argument->defaultParameter.boundingRect().toRect();

		if (!boundingRectangle.contains(
				argument->defaultParameter.mapFromParent(triggerPosition).toPoint()))
			continue;

		actualDx = std::max(boundingRectangle.width() + dx, argument->defaultParameter.m_width)
			- boundingRectangle.width();

		if (dy < 0)
		{
			const auto heighestArgument = std::max_element(m_arguments.cbegin(), m_arguments.cend(),
				[&](const auto& lhs, const auto& rhs)
				{
					// Don't consider the trigger argument itself
					if (&lhs == &*argument)
						return true;

					if (&rhs == &*argument)
						return false;

					// Only consider arguments with parameters
					if (!lhs.parameter && !rhs.parameter)
						return false;

					if (!lhs.parameter)
						return true;

					if (!rhs.parameter)
						return false;

					return lhs.parameter->m_height < rhs.parameter->m_height;
				});

			if (heighestArgument != m_arguments.cend() && heighestArgument->parameter)
			{
				if (heighestArgument->parameter->m_height + 2 * s_margin > m_height + dy)
					actualDy = heighestArgument->parameter->m_height + 2 * s_margin - m_height;
			}
			else
			{
				const auto heighestDefaultArgument = std::max_element(
					m_arguments.cbegin(), m_arguments.cend(),
					[&](const auto& lhs, const auto& rhs)
					{
						return lhs.defaultParameter.m_height < rhs.defaultParameter.m_height;
					});

				if (heighestDefaultArgument != m_arguments.cend()
						&& heighestDefaultArgument->defaultParameter.m_height + 2 * s_margin
							> m_height + dy)
					actualDy = heighestDefaultArgument->defaultParameter.m_height + 2 * s_margin
						- m_height;
			}
		}

		if (!actualDx && !actualDy)
			return QPoint();

		moveParameters(argument + 1, actualDx);

		break;
	}

	return QPoint(actualDx, actualDy);
}

void ArgumentItem::addParameter(Parameter& parameter, Argument& argument,
	const QPoint &offset)
{
	assert(!argument.parameter && "Parameter of the argument is already assigned");

	auto dx = parameter.m_width - argument.defaultParameter.m_width;
	auto dy = parameter.m_height - argument.defaultParameter.m_height;

	parameter.setParent(this);
	parameter.m_parentReference = reinterpret_cast<Parameter**>(&argument.parameter);
	parameter.setPos(offset);

	resizeBy(dx, dy, offset);

	argument.parameter = &parameter;
}

} // namespace Scratch
