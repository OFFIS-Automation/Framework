// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#ifndef PROCESSORITEM_H
#define PROCESSORITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QPainter>
#include <QGraphicsScene>

#include <QPixmap>
#include <QGraphicsItem>
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPolygonF>

#include "Arrow.h"

class Arrow;
struct ProcessorInfo;

/*
  This class represents an item in the scene: processor,
  join or buffer. Each item can have a list of arrows, connected
  to other items.
  */
class ProcessorItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };

    ProcessorItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void removeArrow(Arrow *arrow);
    void removeArrows();
    void addArrow(Arrow *arrow);
    void update(const ProcessorInfo& info);
    void setPosition(const QPointF &pos);
    void remove();

    QPixmap image() const;
    QPolygonF polygon() const
        { return myPolygon; }
    QString getName() const
        { return mNameItem->toPlainText(); }
    QList<Arrow *> getArrows() const
        { return mArrows; }
    int type() const
        { return Type;}

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QPolygonF myPolygon;
    QList<Arrow *> mArrows;
    QGraphicsTextItem *mNameItem, *mPriorityItem;
};

#endif // PROCESSORITEM_H
