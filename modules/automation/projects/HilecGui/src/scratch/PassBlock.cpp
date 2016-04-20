#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include "PassBlock.h"

namespace Scratch
{

PassBlock::PassBlock()
	: Block(100, 40)
{
	setAcceptDrops(true);
}

void PassBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon polygon;

	polygon << QPoint(0, 0);

	addConnector(polygon,
		0,
		0);

	polygon
		<< QPoint(m_width, 0)
		<< QPoint(m_width, m_height);

	addConnector(polygon,
		0,
		m_height,
		true);

	polygon
		<< QPoint(0, m_height);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

	painter->setPen(m_textStyle);
	painter->drawText(QRectF(m_width / 2 - 10, 10, m_width - 20, m_height - 20), "pass");
}

void PassBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	event->accept();
	event->setDropAction(Qt::IgnoreAction);

	Item& item = Item::unpackItem(*event);

	if (item.itemType() != Item::Type::Block)
		return;

	Block& block = *reinterpret_cast<Block*>(&item);

	if (isSelfOrAncestor(block))
		return;

	if (!inConnectorActivationRange(position, 0)
			&& !inConnectorActivationRange(position, m_height))
		return;

	if (item.scene() != this->scene())
		event->setDropAction(Qt::CopyAction);
	else
		event->setDropAction(Qt::MoveAction);
}

void PassBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const auto& position = event->pos();

	auto oldHeight = m_height;

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

	auto& block = *reinterpret_cast<Block*>(item);

	if (inConnectorActivationRange(position, 0))
		addAbove(block);
	else if (inConnectorActivationRange(position, oldHeight))
		addBelow(block);
}

Block& PassBlock::clone() const
{
	return *(new PassBlock());
}

void PassBlock::print(std::ostream& stream, unsigned indentationDepth = 0) const
{
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "pass\n";

	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

} // namespace Scratch
