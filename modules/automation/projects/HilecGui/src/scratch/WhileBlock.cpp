#include <stdexcept>
#include <ostream>

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include "WhileBlock.h"

namespace Scratch
{

WhileBlock::WhileBlock()
	: ControlFlowBlock(100, 100)
{

	setAcceptDrops(true);
}

int WhileBlock::defaultBodyHeight() const
{
	return m_defaultHeight - 2 * s_defaultHeaderHeight;
}

void WhileBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon polygon;

	polygon << QPoint(0, 0);

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
	painter->drawText(QRectF(10, 10, m_width - 20, s_shaftHeight), "while");
}

void WhileBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	auto& block = Block::unpackBlock(*event);

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	if (isSelfOrAncestor(block))
		return;

	if (!inConnectorActivationRange(position, 0)
			&& !inConnectorActivationRange(position, m_height)
			&& !inConnectorActivationRange(position, m_headerHeight))
		return;

	if (block.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);
}

void WhileBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	auto oldHeight = m_height;
	auto oldHeaderHeight = m_headerHeight;

	auto* block = &Block::unpackBlock(*event);

	event->accept();

	if (block->scene() != this->scene())
	{
		event->setDropAction(Qt::CopyAction);

		block = &block->clone();
	}
	else
	{
		event->setDropAction(Qt::MoveAction);
		block->remove();
	}

	// Destination
	if (inConnectorActivationRange(position, 0))
		addAbove(*block);
	else if (inConnectorActivationRange(position, oldHeight))
		addBelow(*block);
	else if (inConnectorActivationRange(position, oldHeaderHeight))
		addBody(*block, m_body, QPoint(s_shaftHeight, m_headerHeight));
}

Block& WhileBlock::clone() const
{
	return *(new WhileBlock());
}

void WhileBlock::print(std::ostream& stream, unsigned indentationDepth = 0) const
{
	if (!m_condition)
		throw std::invalid_argument("Not all conditions are defined.");

	if (!m_body)
		throw std::invalid_argument("Not all loop bodies are defined.");

	// while
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "while " << *m_condition << ":\n";

	m_body->print(stream, indentationDepth + 1);

	// next
	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

} // namespace Scratch
