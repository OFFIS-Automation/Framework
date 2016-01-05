// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#include <QtGui>

#include "ProcessorItem.h"
#include "Arrow.h"

#include <core/ProcessorInfo.h>

ProcessorItem::ProcessorItem(QGraphicsItem *parent, QGraphicsScene * /*scene */)
    : QGraphicsPolygonItem(parent)
{

    myPolygon << QPointF(-50, -30) << QPointF(50, -30)
              << QPointF(50, 30) << QPointF(-50, 30)
              << QPointF(-50, -30);
    setPolygon(myPolygon);
    mNameItem = new QGraphicsTextItem(this);
    mNameItem->setParentItem(this);
    mPriorityItem = new QGraphicsTextItem(this);
    mPriorityItem->setParentItem(this);
    QFont f;
    f.setPointSize(7);
    mPriorityItem->setFont(f);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setBrush(Qt::white);
}

void ProcessorItem::removeArrow(Arrow *arrow)
{
    int index = mArrows.indexOf(arrow);

    if (index != -1)
        mArrows.removeAt(index);
}

void ProcessorItem::removeArrows()
{
    foreach (Arrow *arrow, mArrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void ProcessorItem::addArrow(Arrow *arrow)
{
    mArrows.append(arrow);
}

QPixmap ProcessorItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}

QVariant ProcessorItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, mArrows) {
            arrow->updatePosition();
        }
    }

    return value;
}

void ProcessorItem::update(const ProcessorInfo &info)
{
    mNameItem->setPlainText(info.name);
    mNameItem->adjustSize();
    int w = mNameItem->boundingRect().width();
    mNameItem->setPos(-w/2, -25);
    if(info.pausedStartup)
        mNameItem->setDefaultTextColor(Qt::darkGray);
    else
        mNameItem->setDefaultTextColor(Qt::black);
    QString prioString;
    QColor color = Qt::darkGreen;
    switch(info.priority)
    {
    case QThread::IdlePriority:
        prioString = ("Idle");
        color = QRgb(0x0003ff);
        break;
    case QThread::LowestPriority:
        prioString = ("Lowest");
        color = QRgb(0x00c6ff);
        break;
    case QThread::LowPriority:
        prioString = ("Low");
        color = QRgb(0x000ff50);
        break;
    case QThread::HighPriority:
        prioString = ("High");
        color = QRgb(0xaaaa00);

        break;
    case QThread::HighestPriority:
        prioString = ("Highest");
        color = QRgb(0xff5b00);
        break;
    case QThread::TimeCriticalPriority:
        prioString = ("Time critical");
        color = QRgb(0xff0000);
        break;
    default:
        prioString = ("Normal");
        color = QRgb(0x1aAA00);
        break;
    }
    mPriorityItem->setDefaultTextColor(color);
    mPriorityItem->setPlainText(QObject::tr("%1 priority").arg(prioString));
    mPriorityItem->adjustSize();
    w = mPriorityItem->boundingRect().width();
    mPriorityItem->setPos(-w/2, -10);
}


void ProcessorItem::setPosition(const QPointF &pos)
{
    setPos(pos);
}

void ProcessorItem::remove()
{
    removeArrows();
    scene()->removeItem(mNameItem);
    scene()->removeItem(this);
}
