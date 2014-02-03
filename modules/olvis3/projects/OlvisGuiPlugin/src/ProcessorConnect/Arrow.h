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

#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>

#include "ProcessorItem.h"

class ProcessorItem;

const qreal Pi = 3.14;

/*
  This class represents an arrow in the scene as a connection
  between two diagram items.
  */
class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(ProcessorItem *startItem, ProcessorItem *endItem,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int getType() const
        { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    ProcessorItem *startItem() const
        { return myStartItem; }
    ProcessorItem *endItem() const
        { return myEndItem; }

public slots:
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    ProcessorItem *myStartItem;
    ProcessorItem *myEndItem;
    QPolygonF arrowHead;
};

#endif // ARROW_H
