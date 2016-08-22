#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QApplication>
#include <QMimeData>
#include <QByteArray>
#include <QDrag>
#include <QGraphicsView>

#include "Item.h"

namespace Scratch
{

const std::unordered_map<std::string, Item::Type> Item::nameToItemType =
	{{"Block", Item::Type::Block}, {"Number", Item::Type::Number},
	{"Condition", Item::Type::Condition}, {"Point", Item::Type::Point}};

Item& Item::unpackItem(const QGraphicsSceneDragDropEvent& event)
{
	auto& byteArray = event.mimeData()->data("Item");

	return **reinterpret_cast<Item**>(byteArray.data());
}

Item::Item()
{
	setAcceptDrops(true);

	m_textStyle.setBrush(Qt::white);

	m_outlineStyle.setBrush(Qt::white);
	m_outlineStyle.setWidth(4);

	m_fillStyle = Qt::lightGray;

	updateItem();
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
	if (m_parent)
	{
		item.setParent(m_parent);
		m_parent->updateItem();
	}
	else
		scene()->addItem(&item);
}

int Item::defaultHeight()
{
	return 2 * s_margin + QFontMetrics(m_font).height();
}

bool Item::updateItem()
{
	auto oldWidth = m_width;
	auto oldHeight = m_height;

	m_width = 2 * defaultHeight();
	m_height = defaultHeight();

	return oldWidth != m_width || oldHeight != m_height;
}

} // namespace Scratch
