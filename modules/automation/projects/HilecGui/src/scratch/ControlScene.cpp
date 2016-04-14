#include "ControlScene.h"

#include <QGraphicsSceneEvent>

namespace Scratch
{

ControlScene::ControlScene(QObject* parent)
	: QGraphicsScene(parent)
{}

void ControlScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	QGraphicsScene::dragMoveEvent(event);

	event->ignore();
}

void ControlScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	event->ignore();
}

} // namespace Scratch
