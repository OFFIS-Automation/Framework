#include <stdexcept>
#include <ostream>

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include "IfElseBlock.h"

namespace Scratch
{

IfElseBlock::IfElseBlock()
	: ControlFlowBlock(100, 150, 2),
	  m_trueBody{m_bodies.at(0)},
	  m_falseBody{m_bodies.at(1)}
{
	setAcceptDrops(true);
}

int IfElseBlock::defaultBodyHeight() const
{
	return (m_defaultHeight - 3 * s_defaultHeaderHeight) / 2;
}

void IfElseBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon polygon;

	polygon	<< QPoint(0, 0);

	addConnector(polygon,
		0,
		0);

	polygon
		<< QPoint(m_width, 0)
		<< QPoint(m_width, m_headerHeight);

	addConnector(polygon,
		s_shaftHeight,
		m_headerHeight,
		true);

	polygon
		<< QPoint(s_shaftHeight, m_headerHeight)
		<< QPoint(s_shaftHeight, m_headerHeight + m_trueBodyHeight);

	addConnector(polygon,
		s_shaftHeight,
		m_headerHeight + m_trueBodyHeight);

	polygon
		<< QPoint(m_width, m_headerHeight + m_trueBodyHeight)
		<< QPoint(m_width, m_headerHeight + m_trueBodyHeight + s_defaultHeaderHeight);

	addConnector(polygon,
		s_shaftHeight,
		m_headerHeight + m_trueBodyHeight + s_defaultHeaderHeight,
		true);

	polygon
		<< QPoint(s_shaftHeight, m_headerHeight + m_trueBodyHeight + s_defaultHeaderHeight)
		<< QPoint(s_shaftHeight, m_height - s_defaultHeaderHeight);

	addConnector(polygon,
		s_shaftHeight,
		m_height - s_defaultHeaderHeight);

	polygon
		<< QPoint(m_width, m_height - s_defaultHeaderHeight)
		<< QPoint(m_width, m_height);

	addConnector(polygon,
		0,
		m_height,
		true);

	polygon	<< QPoint(0, m_height);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

	painter->setPen(m_textStyle);
	painter->drawText(QRectF(10, 10, m_width - 20, s_shaftHeight), "if");
	painter->drawText(QRectF(
		10, m_headerHeight + m_trueBodyHeight + 10, m_width - 20, s_shaftHeight), "else");
}

void IfElseBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() == Item::Type::Condition)
	{
		if (!inConditionArea(position))
			return;

		if (m_condition)
			return;
	}
	else if (item.itemType() == Item::Type::Block)
	{
		Block& block = *reinterpret_cast<Block*>(&item);

		if (isSelfOrAncestor(block))
			return;

		if (!inConnectorActivationRange(position, 0)
				&& !inConnectorActivationRange(position, m_height)
				&& !inConnectorActivationRange(position, m_headerHeight)
				&& !inConnectorActivationRange(position,
					m_headerHeight + m_trueBodyHeight + s_shaftHeight))
			return;
	}
	else
		return;

	if (item.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);
}

void IfElseBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	auto oldHeight = m_height;
	auto oldHeaderHeight = m_headerHeight;
	auto oldTrueBodyHeight = m_trueBodyHeight;

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
		else if (inConnectorActivationRange(position, oldHeaderHeight))
			addBody(block, m_trueBody, QPoint(s_shaftHeight, m_headerHeight));
		else if (inConnectorActivationRange(position,
				oldHeaderHeight + oldTrueBodyHeight + s_shaftHeight))
			addBody(block, m_falseBody,
				QPoint(s_shaftHeight, m_headerHeight + m_trueBodyHeight + s_shaftHeight));
	}
}

Block& IfElseBlock::clone() const
{
	return *(new IfElseBlock());
}

void IfElseBlock::print(std::ostream& stream, unsigned indentationDepth = 0) const
{
	if (!m_condition)
		throw std::invalid_argument("Not all conditions are defined.");

	if (!m_trueBody || !m_falseBody)
		throw std::invalid_argument("Not all if bodies are defined.");

	// if

	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "if " << *m_condition << ":\n";

	m_trueBody->print(stream, indentationDepth + 1);

	// else

	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "else:\n";

	m_falseBody->print(stream, indentationDepth + 1);

	// next

	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

void IfElseBlock::resizeBy(int dx, int dy, const QPointF& triggerPosition)
{
	int actualDy = dy;

	if (!inConditionArea(triggerPosition)
		&& triggerPosition.y() <= m_headerHeight + m_trueBodyHeight)
	{
		actualDy = std::max(m_trueBodyHeight + dy, defaultBodyHeight()) - m_trueBodyHeight;

		if (!actualDy)
			return;

		m_trueBodyHeight += actualDy;

		update(boundingRect());

		if (m_falseBody)
		{
			m_falseBody->moveBy(0, actualDy);
			m_falseBody->updateSuccessorPositions(0, actualDy);
		}
	}
	else
	{
		int falseBodyHeight = m_height - m_headerHeight - m_trueBodyHeight
			- 2 * s_defaultHeaderHeight;

		actualDy = std::max(falseBodyHeight + dy, defaultBodyHeight()) - falseBodyHeight;

		if (!actualDy)
			return;
	}

	ControlFlowBlock::resizeBy(dx, actualDy, triggerPosition);
}

} // namespace Scratch
