#include "ProgramScene.h"

#include "Block.h"

#include <QGraphicsSceneEvent>
#include <QGraphicsView>

#include <QMimeData>
#include <QByteArray>

#include <iostream>

namespace Scratch
{

ProgramScene::ProgramScene(QObject* parent)
	: QGraphicsScene(parent)
{}

void ProgramScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	if (!m_firstBlock)
		return;

	QGraphicsScene::dragMoveEvent(event);
}

void ProgramScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	if (m_firstBlock)
	{
		QGraphicsScene::dropEvent(event);

		return;
	}

	auto byteArray = event->mimeData()->data("Block");
	auto* item = *reinterpret_cast<Block**>(byteArray.data());

	m_firstBlock = &item->clone();
	m_firstBlock->setPredecessorsReference(&m_firstBlock);

	addItem(m_firstBlock);
}

} // namespace Scratch
