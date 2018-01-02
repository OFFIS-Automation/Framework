// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <QMessageBox>
#include <QKeyEvent>

#include "DiagramScene.h"
#include "Arrow.h"
#include "ChangeProcessorDialog.h"

#include <src/OlvisSingleton.h>

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    reset();
    const OlvisInterface& controller = OlvisSingleton::instance();

    connect(&controller, SIGNAL(processorCreated(ProcessorInfo)), SLOT(createProcessorItem(ProcessorInfo)));
    connect(this, SIGNAL(deleteProcessorRequested(int)), &controller, SLOT(deleteProcessor(int)), Qt::QueuedConnection);
    connect(&controller, SIGNAL(processorDeleted(ProcessorInfo)), SLOT(deleteProcessorItem(ProcessorInfo)));
    connect(&controller, SIGNAL(processorUpdated(ProcessorInfo)), SLOT(updateProcessorItem(ProcessorInfo)));

    connect(this, SIGNAL(createConnectionRequested(int,int)), &controller, SLOT(connectProcessor(int,int)), Qt::QueuedConnection);
    connect(&controller, SIGNAL(processorsConnected(int,int)), SLOT(createConnection(int,int)));

    connect(this, SIGNAL(deleteConnectionRequested(int,int)), &controller, SLOT(disconnectProcessor(int,int)), Qt::QueuedConnection);
    connect(&controller, SIGNAL(processorsDisconnected(int,int)), SLOT(deleteConnection(int,int)));

    connect(this, SIGNAL(selectionChanged()), SLOT(onSelectionChanged()));
}

void DiagramScene::reset()
{
    mDrawingLine = 0;
    mMode = MoveItem;
}

void DiagramScene::createProcessorItem(ProcessorInfo pinfo)
{
    ProcessorItem *item = new ProcessorItem();
    item->update(pinfo);
    addItem(item);
    mItemMap.insert(pinfo.id, item);
    item->setPosition(QPointF(0,0));
    while(item->collidingItems().size() > item->childItems().size())
    {
        QPointF pos = item->pos();
        pos += QPoint(30, 0);
        if(pos.x() > 300)
        {
            pos.setX(0);
            pos += (QPointF(0, 100));
        }
        item->setPos(pos);
    }


    update();
}

void DiagramScene::deleteProcessorItem(ProcessorInfo pinfo)
{
    mItemMap.value(pinfo.id)->remove();
    mItemMap.remove(pinfo.id);
    update();
}


void DiagramScene::updateProcessorItem(ProcessorInfo pinfo)
{
    ProcessorItem *item = mItemMap.value(pinfo.id);
    if(item)
        item->update(pinfo);
}

void DiagramScene::createConnection(int source, int target)
{
    if(!mItemMap.contains(source) || !mItemMap.contains(target))
        return;         // should not happen
    ProcessorItem *startItem = mItemMap.value(source);
    ProcessorItem *endItem = mItemMap.value(target);
    Arrow *arrow = new Arrow(startItem, endItem);
    ArrowKey key(source,target);
    mArrowMap[key] = arrow;
    startItem->addArrow(arrow);
    endItem->addArrow(arrow);
    arrow->setZValue(-1000.0); // behind all items
    addItem(arrow);
    arrow->updatePosition();
    update();
}

void DiagramScene::deleteConnection(int source, int target)
{
    if(!mItemMap.contains(source) || !mItemMap.contains(target))
        return;         // should not happen
    ProcessorItem *startItem = mItemMap.value(source);
    ProcessorItem *endItem = mItemMap.value(target);
    ArrowKey key(source,target);
    if(!mArrowMap.contains(key)) // should not happen
        return;
    startItem->removeArrow(mArrowMap[key]);
    endItem->removeArrow(mArrowMap[key]);
    removeItem(mArrowMap[key]);
    delete mArrowMap[key];
    update();
}

void DiagramScene::setPosition(const QString &name, QPoint position)
{
    foreach(ProcessorItem* item, mItemMap.values())
    {
        if(item->getName() == name)
        {
            item->setPosition(position);
            break;
        }
    }
    update();
}


void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::RightButton)
        mMode = InsertLine;
    else if (mouseEvent->button() != Qt::LeftButton)
        return;

    switch (mMode)
    {
    case InsertLine:
        mDrawingLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
        mDrawingLine->setPen(QPen(Qt::black, 2));
        addItem(mDrawingLine);
        break;

    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}


void DiagramScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() != Qt::LeftButton)
        return;

    if (mMode == MoveItem && !selectedItems().isEmpty())
    {
        if (selectedItems().first()->type() == ProcessorItem::Type)
        {
            ProcessorItem *item = (ProcessorItem*)selectedItems().first();
            if(mItemMap.key(item) == 0)
                return;     // should not happen
            int id = mItemMap.key(item);
            ChangeProcessorDialog dialog(id);
            if (dialog.exec())
                dialog.updateProcessor();
        }
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}


void DiagramScene::onSelectionChanged()
{
    if (mMode == MoveItem && selectedItems().size() == 1)
    {
        if (selectedItems().first()->type() == ProcessorItem::Type)
        {
            ProcessorItem *item = (ProcessorItem*)selectedItems().first();
            if(mItemMap.key(item) == 0)
                return;     // should not happen
            int id = mItemMap.key(item);
            emit processorSelected(id);
        }
    }
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mMode == InsertLine && mDrawingLine != 0) {
        QList<QGraphicsItem *> startItems = items(mDrawingLine->line().p1());
        if (startItems.count() && startItems.first() == mDrawingLine)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(mDrawingLine->line().p2());
        if (endItems.count() && endItems.first() == mDrawingLine)
            endItems.removeFirst();

        mDrawingLine->setPen(QPen(Qt::yellow, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        if (startItems.count() > 0 && endItems.count() > 0 &&
                    (startItems.first()->type() == ProcessorItem::Type || startItems.first()->type() == QGraphicsTextItem::Type) &&
                    (endItems.first()->type() == ProcessorItem::Type || endItems.first()->type() == QGraphicsTextItem::Type) &&
                    startItems.first() != endItems.first())
        {
            ProcessorItem *startItem, *endItem;
            if (startItems.first()->type() == QGraphicsTextItem::Type) {
                startItem =
                        qgraphicsitem_cast<ProcessorItem *>(startItems.first()->parentItem());
            } else {
                startItem =
                        qgraphicsitem_cast<ProcessorItem *>(startItems.first());
            }
            if (endItems.first()->type() == QGraphicsTextItem::Type) {
                endItem =
                        qgraphicsitem_cast<ProcessorItem *>(endItems.first()->parentItem());
            } else {
                endItem =
                        qgraphicsitem_cast<ProcessorItem *>(endItems.first());
            }

            const OlvisInterface& controller = OlvisSingleton::instance();
            if (controller.canConnectProcessors(mItemMap.key(startItem), mItemMap.key(endItem)))
            {
                mDrawingLine->setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            } else
            {
                mDrawingLine->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            }
        }

        QLineF newLine(mDrawingLine->line().p1(), mouseEvent->scenePos());
        mDrawingLine->setLine(newLine);
    } else if (mMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }

}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mDrawingLine != 0 && mMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(mDrawingLine->line().p1());
        if (startItems.count() && startItems.first() == mDrawingLine)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(mDrawingLine->line().p2());
        if (endItems.count() && endItems.first() == mDrawingLine)
            endItems.removeFirst();

        removeItem(mDrawingLine);
        delete mDrawingLine;

        if (startItems.count() > 0 && endItems.count() > 0 &&
                (startItems.first()->type() == ProcessorItem::Type || startItems.first()->type() == QGraphicsTextItem::Type) &&
                (endItems.first()->type() == ProcessorItem::Type || endItems.first()->type() == QGraphicsTextItem::Type) &&
                startItems.first() != endItems.first())
        {
            ProcessorItem *startItem, *endItem;
            if (startItems.first()->type() == QGraphicsTextItem::Type) {
                startItem =
                        qgraphicsitem_cast<ProcessorItem *>(startItems.first()->parentItem());
            } else {
                startItem =
                        qgraphicsitem_cast<ProcessorItem *>(startItems.first());
            }
            if (endItems.first()->type() == QGraphicsTextItem::Type) {
                endItem =
                        qgraphicsitem_cast<ProcessorItem *>(endItems.first()->parentItem());
            } else {
                endItem =
                        qgraphicsitem_cast<ProcessorItem *>(endItems.first());
            }

            if(mItemMap.key(startItem) == 0 || mItemMap.key(endItem) == 0)
                return;     // should not happen

            emit createConnectionRequested(mItemMap.key(startItem), mItemMap.key(endItem));
        }
        mMode = MoveItem;
    }

    mDrawingLine = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    setSceneRect(itemsBoundingRect());
}

void DiagramScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete && !selectedItems().isEmpty())
    {
        if (selectedItems().first()->type() == ProcessorItem::Type)
        {
            ProcessorItem *item = (ProcessorItem*)selectedItems().first();
            int id;

            if(mItemMap.key(item) == 0)
                return;     // should not happen
            if(QMessageBox::question(0, tr("Delete processor"), tr("Do you really want to delete the processor %1?").arg(item->getName()),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
                return;
            id = mItemMap.key(item);
            emit deleteProcessorRequested(id);
        }
        else
        {
            Arrow* arrow = (Arrow*)selectedItems().first();
            if(arrow)
            {
                ArrowKey key = mArrowMap.key(arrow);
                emit deleteConnectionRequested(key.first, key.second);
            }

        }
    }
}

void DiagramScene::setMode(Mode mode)
{
    mMode = mode;
}
