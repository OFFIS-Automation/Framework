#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QApplication>
#include <QMimeData>
#include <QByteArray>
#include <QDrag>

#include "Item.h"

namespace Scratch
{

Item& Item::unpackItem(const QGraphicsSceneDragDropEvent& event)
{
	auto& byteArray = event.mimeData()->data("Item");

	return **reinterpret_cast<Item**>(byteArray.data());
}

Item::Item(const int width, const int height)
	: m_width(width),
	  m_height(height),
	  m_defaultWidth{width},
	  m_defaultHeight{height}
{
	m_textStyle.setBrush(Qt::white);

	m_outlineStyle.setBrush(Qt::white);
	m_outlineStyle.setWidth(4);

	m_fillStyle = Qt::lightGray;
}

void Item::setParent(Item* parent)
{
	m_parent = parent;

	setParentItem(parent);
}

void Item::remove()
{
	scene()->removeItem(this);

	setParent(nullptr);
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->accept();
}

void Item::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	event->accept();

	if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length()
			< QApplication::startDragDistance())
		return;

	QMimeData* mimeData = new QMimeData;

	auto* item = this;
	QByteArray byteArray(reinterpret_cast<char*>(&item), sizeof(Item*));
	mimeData->setData("Item", byteArray);

	QDrag* drag = new QDrag(event->widget());
	drag->setMimeData(mimeData);
	drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void Item::addItem(Item& item)
{
	item.setPos(pos());

	if (m_parent)
	{
		item.setParent(m_parent);
		m_parent->resizeBy(0, item.m_height, pos().toPoint());
	}
	else
		scene()->addItem(&item);
}

QPoint Item::resizeBy(int dx, int dy, const QPoint&)
{
	const auto actualDx = std::max(m_width + dx, m_defaultWidth) - m_width;
	const auto actualDy = std::max(m_height + dy, m_defaultHeight)- m_height;

	if (!actualDx && !actualDy)
		return QPoint();

	prepareGeometryChange();

	m_width += actualDx;
	m_height += actualDy;

	if (m_parent)
		m_parent->resizeBy(0, actualDy, pos().toPoint());

	return QPoint(actualDx, actualDy);
}

} // namespace Scratch
