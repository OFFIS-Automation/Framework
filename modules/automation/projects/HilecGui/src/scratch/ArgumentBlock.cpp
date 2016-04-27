#include "ArgumentBlock.h"

#include <algorithm>

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsScene>

#include "Condition.h"

namespace Scratch
{

ArgumentBlock::ArgumentBlock(const std::string& name)
	: Block(s_defaultWidth, s_defaultHeight),
	  m_name{name}
{
	m_width = QFontMetrics(m_font).width(QString::fromStdString(name)) + 2 * s_margin;

	setAcceptDrops(true);
}

void ArgumentBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*,
	QWidget*)
{
	QPolygon polygon;
	drawOutline(polygon);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

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
		{
			x += argument.parameter->m_width + s_margin;

			continue;
		}

		polygon.clear();

		if (argument.type == Item::Type::Condition)
			Condition::drawOutline(polygon,
				QRect(x, s_margin, s_defaultParameterWidth, parameterHeight()));

		painter->setBrush(m_outlineStyle.color());
		painter->setPen(m_outlineStyle);
		painter->setRenderHint(QPainter::Antialiasing);
		painter->drawPolygon(polygon);

		x += s_defaultParameterWidth + s_margin;
	}
}

Item& ArgumentBlock::clone() const
{
	auto& argumentBlock =  *(new ArgumentBlock(m_name));

	for (const auto& argument : m_arguments)
		argumentBlock.addArgument(argument.name, argument.type);

	return argumentBlock;
}

void ArgumentBlock::print(std::ostream& stream, unsigned indentationDepth) const
{
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "foo." << m_name << "(";

	for (auto argument = m_arguments.cbegin(); argument != m_arguments.cend(); ++argument)
	{
		if (!argument->parameter)
			throw std::invalid_argument("Not all parameters are defined.");

		stream << *argument->parameter;

		if (argument + 1 != m_arguments.cend())
			stream << ", ";
	}

	stream << ")\n";

	// next
	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

void ArgumentBlock::addArgument(const std::string& name, const Item::Type& type)
{
	QFontMetrics fontMetric(m_font);

	const auto appendageWidth =
		s_margin + fontMetric.width(QString::fromStdString(name + ":")) + s_margin / 2;

	const auto x = (m_arguments.empty()?
			s_margin + fontMetric.width(QString::fromStdString(m_name)) :
			m_arguments.back().x + (m_arguments.back().parameter?
				m_arguments.back().parameter->m_width :
				s_defaultParameterWidth))
		 + appendageWidth;

	m_arguments.push_back({name, type, x, nullptr});
	m_width += appendageWidth + s_defaultParameterWidth;
}

void ArgumentBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() == Item::Type::Block)
	{
		if (!inConnectorActivationRange(position, 0)
				&& !inConnectorActivationRange(position, m_height))
			return;
	}
	else
	{
		Parameter& parameter = *reinterpret_cast<Parameter*>(&item);

		const auto argument =
			std::find_if(m_arguments.cbegin(), m_arguments.cend(), [&](const auto& argument)
				{
					if (argument.parameter)
						return false;

					if (parameter.itemType() != argument.type)
						return false;

					const auto& boundingRectangle = QRect(
						argument.x, s_margin,
						s_defaultParameterWidth, parameterHeight());

					return boundingRectangle.contains(position.toPoint());
				});

		if (argument == m_arguments.cend())
			return;
	}

	if (item.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);
}

void ArgumentBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
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

	// Block
	if (item->itemType() == Item::Type::Block)
	{
		auto& block = *reinterpret_cast<Block*>(item);

		if (inConnectorActivationRange(position, 0))
			addAbove(block);
		else if (inConnectorActivationRange(position, oldHeight))
			addBelow(block);
	}
	// Parameter
	else
	{
		auto& parameter = *reinterpret_cast<Parameter*>(item);

		auto oldArgument = oldArguments.cbegin();

		for (auto &argument : m_arguments)
		{
			const auto& boundingRectangle =
				QRect(oldArgument->x, s_margin, s_defaultParameterWidth, oldHeight - 2 * s_margin);

			if (boundingRectangle.contains(position.toPoint()))
			{
				addParameter(parameter, argument.parameter, QPoint(argument.x, s_margin));

				return;
			}

			++oldArgument;
		}
	}
}

void ArgumentBlock::resizeBy(int dx, int dy, const QPoint& triggerPosition)
{
	auto actualDx = dx;
	auto actualDy = dy;

	auto moveParameters = [&](auto&& start, auto&& dx)
	{
		std::for_each(start, m_arguments.end(), [&](auto& argument)
		{
			argument.x += dx;

			if (argument.parameter)
				argument.parameter->moveBy(dx, 0);
		});
	};

	for (auto argument = m_arguments.begin(); argument != m_arguments.end(); ++argument)
	{
		const auto& boundingRectangle = argument->parameter?
			argument->parameter->mapRectToParent(argument->parameter->boundingRect()).toRect() :
			QRect(
				argument->x, s_margin,
				s_defaultParameterWidth, parameterHeight());

		if (!boundingRectangle.contains(triggerPosition))
			continue;

		actualDx = std::max(boundingRectangle.width() + dx, s_defaultParameterWidth)
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

			if (heighestArgument != m_arguments.cend() && heighestArgument->parameter
				&& heighestArgument->parameter->m_height + 2 * s_margin > m_height + dy)
					actualDy = heighestArgument->parameter->m_height + 2 * s_margin - m_height;
		}

		if (!actualDx && !actualDy)
			return;

		moveParameters(argument + 1, actualDx);

		break;
	}

	Block::resizeBy(actualDx, actualDy, triggerPosition);
}

void ArgumentBlock::addParameter(Parameter& parameter, Parameter*& argumentParameter,
	const QPoint &offset)
{
	assert(!argumentParameter && "Parameter of the argument is already assigned");

	const auto dx = parameter.m_width - s_defaultParameterWidth;
	const auto dy = parameter.m_height - parameterHeight();

	parameter.setParent(this);
	parameter.m_parentReference = reinterpret_cast<Parameter**>(&argumentParameter);
	parameter.setPos(offset);

	resizeBy(dx, dy, offset);

	argumentParameter = &parameter;
}

} // namespace Scratch
