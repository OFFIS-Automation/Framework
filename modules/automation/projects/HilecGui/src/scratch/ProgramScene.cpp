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
	{
		event->setDropAction(Qt::CopyAction);
		event->accept();

		return;
	}

	QGraphicsScene::dragMoveEvent(event);
}

void ProgramScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	if (m_firstBlock)
	{
		QGraphicsScene::dropEvent(event);

		return;
	}

	event->setDropAction(Qt::CopyAction);
	event->accept();

	auto& item = Block::unpackBlock(*event);

	m_firstBlock = &item.clone();
	m_firstBlock->setPredecessorsReference(&m_firstBlock);

	addItem(m_firstBlock);
}

} // namespace Scratch
