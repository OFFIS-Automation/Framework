#ifndef SCRATCHCONTROLFLOWBLOCK_H
#define SCRATCHCONTROLFLOWBLOCK_H

#include <ostream>
#include <memory>
#include <array>
#include <algorithm>

#include <QGraphicsSceneEvent>

#include "Block.h"
#include "Condition.h"

namespace Scratch
{

template <size_t numberOfBodies>
class ControlFlowBlock : public Block
{
	protected:
		static const int s_shaftHeight = 30;
		static const int s_defaultHeaderHeight = s_shaftHeight;

		static const int s_conditionMargin = 10;

		constexpr int defaultConditionWidth() const
		{
			return m_defaultWidth - s_shaftHeight - 2 * s_conditionMargin;
		}

		constexpr int defaultConditionHeight() const
		{
			return s_defaultHeaderHeight - 2 * s_conditionMargin;
		}

		constexpr int defaultBodyHeight() const
		{
			return (m_defaultHeight - (numberOfBodies + 1) * s_defaultHeaderHeight)
				/ numberOfBodies;
		}

		struct Body
		{
			int height;
			Block* block;
		};

	public:
		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

	protected:
		ControlFlowBlock(const int width, const int height);

		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		bool inConditionArea(const QPointF& position);

		void resizeBy(int dx, int dy, const QPointF& triggerPosition);

		void addCondition(Condition& condition);
		void addBody(Block& block, Block*& bodyBlock, const QPoint &offset);

		int m_headerHeight = s_defaultHeaderHeight;

		Condition* m_condition{nullptr};

		std::array<Body, numberOfBodies> m_bodies{};
};

template <size_t numberOfBodies>
ControlFlowBlock<numberOfBodies>::ControlFlowBlock(const int width, const int height)
	: Block(width, height)
{
	m_bodies.fill({defaultBodyHeight(), nullptr});
}

template <size_t numberOfBodies>
void ControlFlowBlock<numberOfBodies>::paint(QPainter* painter, const QStyleOptionGraphicsItem*,
	QWidget*)
{
	QPolygon polygon;
	drawOutline(polygon);

	auto y = m_headerHeight;

	std::for_each(m_bodies.cbegin(), m_bodies.cend(), [&](const auto& body)
		{
			polygon << QPoint(m_width, y);
			drawConnector(polygon, s_shaftHeight, y, true);
			polygon << QPoint(s_shaftHeight, y);

			y += body.height;

			polygon << QPoint(s_shaftHeight, y);
			drawConnector(polygon, s_shaftHeight, y);
			polygon << QPoint(m_width, y);

			y += s_defaultHeaderHeight;
		});

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

	polygon.clear();

	Condition::drawOutline(polygon, m_width - s_shaftHeight - 2 * s_conditionMargin,
		m_headerHeight - 2 * s_conditionMargin,
		QPoint(s_shaftHeight + s_connectorMargin, s_connectorMargin));

	painter->setBrush(m_outlineStyle.color());
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);
}

template <size_t numberOfBodies>
void ControlFlowBlock<numberOfBodies>::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

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
		event->setDropAction(Qt::MoveAction);
}

template <size_t numberOfBodies>
void ControlFlowBlock<numberOfBodies>::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

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
		auto& condition = *reinterpret_cast<Condition*>(item);

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
	}
}

template <size_t numberOfBodies>
bool ControlFlowBlock<numberOfBodies>::inConditionArea(const QPointF& position)
{
	return QRect(s_shaftHeight + s_connectorMargin, s_connectorMargin,
			m_width - s_shaftHeight - 2 * s_conditionMargin, m_headerHeight - 2 * s_conditionMargin)
		.contains(position.toPoint());
}

template <size_t numberOfBodies>
void ControlFlowBlock<numberOfBodies>::resizeBy(int dx, int dy, const QPointF& triggerPosition)
{
	int actualDx{dx};
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

	auto y = m_headerHeight;

	for (auto body = m_bodies.begin(); body != m_bodies.end(); ++body)
	{
		y += body->height;

		if (triggerPosition.y() > y)
		{
			y += s_defaultHeaderHeight;

			continue;
		}

		actualDy = std::max(body->height + dy, defaultBodyHeight()) - body->height;

		if (!actualDy)
			return;

		body->height += actualDy;
		moveBodies(body + 1, actualDy);

		break;
	}

	Block::resizeBy(actualDx, actualDy, triggerPosition);
}

template <size_t numberOfBodies>
void ControlFlowBlock<numberOfBodies>::addCondition(Condition& condition)
{
	const auto dWidth = condition.m_width - defaultConditionWidth();
	const auto dHeight = condition.m_height - defaultConditionHeight();

	const QPointF offset(s_shaftHeight + s_conditionMargin, s_conditionMargin);

	condition.setParent(this);
	condition.setPos(offset);

	m_condition = &condition;
	resizeBy(dWidth, dHeight, offset);

	scene()->addItem(&condition);
}

template <size_t numberOfBodies>
void ControlFlowBlock<numberOfBodies>::addBody(Block& block, Block*& bodyBlock, const QPoint &offset)
{
	const auto dHeight = block.m_height - !bodyBlock * defaultBodyHeight();

	block.setPredecessorsReference(&bodyBlock);
	block.setSuccessor(bodyBlock);
	block.setParent(this);
	block.setPos(offset);

	bodyBlock = &block;
	resizeBy(0, dHeight, offset);

	scene()->addItem(&block);
}

} // namespace Scratch

#endif // SCRATCHCONTROLFLOWBLOCK_H
