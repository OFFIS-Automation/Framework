#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include "FrameBlocks.h"

namespace Scratch
{

// Frame block

FrameBlock::FrameBlock(const std::string& name)
:	ArgumentItem(name)
{
	updateItem();
}

bool FrameBlock::updateItem()
{
	m_horizontalMargin = s_midsegmentOffset + !m_arguments.empty() * m_outlineStyle.width() / 2.0;
	auto updated = ArgumentItem::updateItem();
	m_horizontalMargin = s_midsegmentOffset;

	updated |= Block::updateItem();

	return updated;
}

void FrameBlock::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->accept();
}

void FrameBlock::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	event->accept();
};

// Start block

StartBlock::StartBlock(const std::string& name)
:	FrameBlock(name)
{}

void StartBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
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

	ArgumentItem::paint(painter, item, widget);
}

void StartBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() != Item::Type::Block)
		return;

	Block& block = *dynamic_cast<Block*>(&item);

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

	auto* block = dynamic_cast<Block*>(&Item::unpackItem(*event));

	event->accept();

	if (block->scene() != this->scene())
	{
		event->setDropAction(Qt::CopyAction);

		block = dynamic_cast<Block*>(&block->clone());
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
	return *(new StartBlock(m_name));
}

void StartBlock::print(std::ostream& stream, unsigned indentationDepth) const
{
	stream << "def ";
	ArgumentItem::print(stream);
	stream << ":" << std::endl;

	if (m_successor)
		m_successor->print(stream, indentationDepth + 1);
}

// End block

EndBlock::EndBlock()
:	FrameBlock("end")
{}

void EndBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
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

	ArgumentItem::paint(painter, item, widget);
}

void EndBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() != Item::Type::Block)
	{
		ArgumentItem::dragMoveEvent(event);

		return;
	}

	Block& block = *dynamic_cast<Block*>(&item);

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

	Item& item = Item::unpackItem(*event);

	if (item.itemType() != Item::Type::Block)
	{
		ArgumentItem::dropEvent(event);

		return;
	}

	auto* block = dynamic_cast<Block*>(&item);

	event->accept();

	if (block->scene() != this->scene())
	{
		event->setDropAction(Qt::CopyAction);

		block = dynamic_cast<Block*>(&block->clone());
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

void EndBlock::print(std::ostream& stream, unsigned indentationDepth) const
{
	printIndentation(stream, indentationDepth);

	stream << "return";

	if (m_arguments.empty())
	{
		stream << std::endl;

		return;
	}

	stream << " ";

	const auto& argument = m_arguments.at(0);

	if (argument.parameter)
		stream << *argument.parameter;
	else
		stream << argument.defaultParameter;

	stream << std::endl;
}

} // namespace Scratch
