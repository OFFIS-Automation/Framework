#include "ArgumentBlock.h"

#include <QPainter>

#include "TrueCondition.h"

namespace Scratch
{

ArgumentBlock::ArgumentBlock()
	: Block(270, 50)
{
	m_name = "Foo";
	m_arguments.push_back({"b", Item::Type::Condition, new TrueCondition()});
	m_arguments.push_back({"a", Item::Type::Condition, new TrueCondition()});
	m_arguments.push_back({"r", Item::Type::Condition, new TrueCondition()});
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

	QFontMetrics fontMetric(painter->font());

	auto x = s_margin;

	auto paintName = [&](auto& name)
	{
		auto position = QPointF(x, m_height / 2 + fontMetric.height() / 4);

		painter->setPen(m_textStyle);
		painter->drawText(position, name);

		x += fontMetric.width(name);
	};

	paintName(QString::fromStdString(m_name));
	x += s_margin;

	std::for_each(m_arguments.cbegin(), m_arguments.cend(), [&](const Argument &argument)
		{
			if (argument.parameter->itemType() == Item::Type::Condition)
			{
				paintName(QString::fromStdString(argument.name + ":"));
				x += s_margin / 2;

				polygon.clear();			
				Condition::drawOutline(polygon, 40, m_height - 2 * s_margin, QPoint(x, s_margin));

				painter->setBrush(m_outlineStyle.color());
				painter->setPen(m_outlineStyle);
				painter->setRenderHint(QPainter::Antialiasing);
				painter->drawPolygon(polygon);

				x += 40 + s_margin;
			}
		});
}

Item& ArgumentBlock::clone() const
{
	return *(new ArgumentBlock());
}

void ArgumentBlock::print(std::ostream& stream, unsigned indentationDepth) const
{

}

void ArgumentBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	/*const auto& position = event->pos();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() == Item::Type::Condition)
	{
		if (!inConditionArea(position))
			return;
	}
	else if (item.itemType() == Item::Type::Block)
	{
		Block& block = *reinterpret_cast<Block*>(&item);

		if (isSelfOrAncestor(block))
			return;

		auto y = m_headerHeight;

		const auto inBodyRange = m_bodies.cend()
			!= std::find_if(m_bodies.cbegin(), m_bodies.cend(), [&](const Body& body)
				{
					const auto& isInRange = inConnectorActivationRange(position, y);

					y += body.height + s_defaultHeaderHeight;

					return isInRange;
				});

		if (!inConnectorActivationRange(position, 0)
				&& !inConnectorActivationRange(position, m_height)
				&& !inBodyRange)
			return;
	}
	else
		return;

	if (item.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);*/
}

void ArgumentBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	/*const auto& position = event->pos();

	auto oldHeight = m_height;
	auto oldHeaderHeight = m_headerHeight;

	auto oldBodies = m_bodies;

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

	// Condition
	if (item->itemType() == Item::Type::Condition)
	{
		auto& condition = *reinterpret_cast<Parameter*>(item);

		addCondition(condition);
	}
	// Block
	else
	{
		auto& block = *reinterpret_cast<Block*>(item);

		if (inConnectorActivationRange(position, 0))
			addAbove(block);
		else if (inConnectorActivationRange(position, oldHeight))
			addBelow(block);
		else
		{
			auto oldY = oldHeaderHeight;
			auto oldBody = oldBodies.cbegin();

			auto y = m_headerHeight;

			for (auto &body : m_bodies)
			{
				if (inConnectorActivationRange(position, oldY))
				{
					addBody(block, body.block, QPoint(s_shaftHeight, y));

					return;
				}

				oldY += oldBody->height + s_defaultHeaderHeight;
				++oldBody;

				y += body.height + s_defaultHeaderHeight;
			}
		}
	}*/
}

void ArgumentBlock::resizeBy(int dx, int dy, const QPointF& triggerPosition)
{
	/*int actualDx{dx};
	int actualDy{dy};

	auto moveBodies = [&](auto &start, auto& dy)
		{
			std::for_each(start, m_bodies.end(), [&](auto& body)
			{
				if (!body.block)
					return;

				body.block->moveBy(0, dy);
				body.block->updateSuccessorPositions(0, dy);
			});
		};

	if (inConditionArea(triggerPosition))
	{
		actualDx = std::max(m_width + dx, m_defaultWidth) - m_width;
		actualDy = std::max(m_headerHeight + dy, s_defaultHeaderHeight) - m_headerHeight;

		if (!actualDx && !actualDy)
			return;

		m_headerHeight += actualDy;
		update(boundingRect());
		moveBodies(m_bodies.begin(), actualDy);

		Block::resizeBy(actualDx, actualDy, triggerPosition);

		return;
	}

	Block::resizeBy(actualDx, actualDy, triggerPosition);*/
}

void ArgumentBlock::addParameter(Parameter& parameter)
{
	/*const auto dWidth = parameter.m_width - defaultConditionWidth();
	const auto dHeight = parameter.m_height - defaultConditionHeight();

	const QPointF offset(s_shaftHeight + s_parameterMargin, s_parameternMargin);

	parameter.setParent(this);
	parameter.setPos(offset);

	m_parameter = &parameter;
	resizeBy(dWidth, dHeight, offset);

	scene()->addItem(&parameter);*/
}

} // namespace Scratch
