#include "ControlFlowBlock.h"

#include <QGraphicsScene>

namespace Scratch
{

ControlFlowBlock::ControlFlowBlock(const int width, const int height, const size_t numberOfBodies)
	: Block(width, height),
	  m_bodies(numberOfBodies)
{}

bool ControlFlowBlock::inConditionArea(const QPointF& position)
{
	return QRect(s_shaftHeight + s_connectorMargin, s_connectorMargin,
			m_width - s_shaftHeight - 2 * s_conditionMargin, m_headerHeight - 2 * s_conditionMargin)
		.contains(position.toPoint());
}

void ControlFlowBlock::resizeBy(int dx, int dy, const QPointF& triggerPosition)
{
	int actualDx = dx;
	int actualDy = dy;

	if (inConditionArea(triggerPosition))
	{
		actualDx = std::max(m_width + dx, m_defaultWidth) - m_width;
		actualDy = std::max(m_headerHeight + dy, s_defaultHeaderHeight) - m_headerHeight;

		if (!actualDx && !actualDy)
			return;

		m_headerHeight += actualDy;

		update(boundingRect());

		std::for_each(m_bodies.begin(), m_bodies.end(), [&](auto& body)
		{
			if (!body)
				return;

			body->moveBy(0, actualDy);
			body->updateSuccessorPositions(0, actualDy);
		});
	}

	Block::resizeBy(actualDx, actualDy, triggerPosition);
}

void ControlFlowBlock::addCondition(Condition& condition)
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

void ControlFlowBlock::addBody(Block& block, Block*& body, const QPoint &offset)
{
	const auto dHeight = block.m_height - !body * defaultBodyHeight();

	block.setPredecessorsReference(&body);
	block.setSuccessor(body);
	block.setParent(this);
	block.setPos(offset);

	body = &block;
	resizeBy(0, dHeight, offset);

	scene()->addItem(&block);
}

} // namespace Scratch
