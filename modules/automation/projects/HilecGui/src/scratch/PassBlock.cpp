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
	drawOutline(polygon);

	painter->setBrush(m_fillStyle);
	painter->setPen(m_outlineStyle);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(polygon);

	painter->setPen(m_textStyle);
	painter->drawText(boundingRect(), Qt::AlignCenter, "Pass");
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
