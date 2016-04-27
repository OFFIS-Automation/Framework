#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include "FrameBlocks.h"

namespace Scratch
{

// Frame block

FrameBlock::FrameBlock()
	: Block(s_defaultWidth, s_defaultHeight)
{
	setAcceptDrops(true);
}

// Start block

StartBlock::StartBlock()
	: FrameBlock()
{
	setAcceptDrops(true);
}

void StartBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon polygon;

	polygon
		<< QPoint(0, s_taperHeight)
		<< QPoint(s_midsegmentOffset, 0)
		<< QPoint(m_width - s_midsegmentOffset, 0)
		<< QPoint(m_width, s_taperHeight)
		<< QPoint(m_width, m_height);

	drawConnector(polygon,
		0,
		m_height,
		true);

	polygon	<< QPoint(0, m_height);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->drawPolygon(polygon);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(boundingRect(), Qt::AlignCenter, "Start");
}

void StartBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() != Item::Type::Block)
		return;

	Block& block = *reinterpret_cast<Block*>(&item);

	if (!inConnectorActivationRange(position, m_height))
		return;

	if (block.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);
}

void StartBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	auto* block = reinterpret_cast<Block*>(&Item::unpackItem(*event));

	event->accept();

	if (block->scene() != this->scene())
	{
		event->setDropAction(Qt::CopyAction);

		block = reinterpret_cast<Block*>(&block->clone());
	}
	else
	{
		event->setDropAction(Qt::MoveAction);
		block->remove();
	}

	if (inConnectorActivationRange(position, m_height))
		addBelow(*block);
}

Block& StartBlock::clone() const
{
	return *(new StartBlock());
}

void StartBlock::print(std::ostream& stream, unsigned indentationDepth) const
{
	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

// End block

EndBlock::EndBlock()
	: FrameBlock()
{
	setAcceptDrops(true);
}

void EndBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon polygon;

	polygon
		<< QPoint(0, 0);

	drawConnector(polygon,
		0,
		0);

	polygon
		<< QPoint(m_width, 0)
		<< QPoint(m_width, m_height - s_taperHeight)
		<< QPoint(m_width - s_midsegmentOffset, m_height)
		<< QPoint(s_midsegmentOffset, m_height)
		<< QPoint(0, m_height - s_taperHeight);

	painter->setPen(m_outlineStyle);
	painter->setBrush(m_fillStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

	painter->setPen(m_textStyle);
	painter->setFont(m_font);
	painter->drawText(boundingRect(), Qt::AlignCenter, "Stop");
}

void EndBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() != Item::Type::Block)
		return;

	Block& block = *reinterpret_cast<Block*>(&item);

	if (!inConnectorActivationRange(position, 0))
		return;

	if (block.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);
}

void EndBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	auto* block = reinterpret_cast<Block*>(&Item::unpackItem(*event));

	event->accept();

	if (block->scene() != this->scene())
	{
		event->setDropAction(Qt::CopyAction);

		block = reinterpret_cast<Block*>(&block->clone());
	}
	else
	{
		event->setDropAction(Qt::MoveAction);
		block->remove();
	}

	if (inConnectorActivationRange(position, 0))
		addAbove(*block);
}

Block& EndBlock::clone() const
{
	return *(new EndBlock());
}

void EndBlock::print(std::ostream&, unsigned) const
{}

} // namespace Scratch
