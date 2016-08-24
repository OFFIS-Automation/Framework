#include "ControlScene.h"

#include <QGraphicsSceneEvent>

#include "../Items/Item.h"

namespace Scratch
{

ControlScene::ControlScene(QObject* parent)
	: QGraphicsScene(parent)
{}

void ControlScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	QGraphicsScene::dragMoveEvent(event);

	auto* item = &Item::unpackItem(*event);

	if (dynamic_cast<ControlScene*>(item->scene()))
	{
		event->accept();
		event->setDropAction(Qt::IgnoreAction);

		return;
	}

	event->accept();
	event->setDropAction(Qt::MoveAction);
}

void ControlScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	auto* item = &Item::unpackItem(*event);

	if (dynamic_cast<ControlScene*>(item->scene()))
	{
		event->accept();
		event->setDropAction(Qt::IgnoreAction);

		return;
	}

	event->accept();
	event->setDropAction(Qt::MoveAction);

	item->remove();
}

} // namespace Scratch
