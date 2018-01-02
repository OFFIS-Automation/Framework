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

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>

#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPointF>
#include <QGraphicsLineItem>
#include <QFont>
#include <QGraphicsTextItem>
#include <QColor>
#include <QMap>

#include "ProcessorItem.h"

#include <core/ProcessorInfo.h>

/*
  This class is the graphics scene of the processors widget,
  in that the diagram items (processor, join, buffer) and their
  connections are drawn.
  */
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /*
      The different modes of the scene. Default mode is MoveItem. There
      you can move items with the mouse.
      It's switched to InsertItem if a button was selected. Left-clicking
      places the item in the scene.
      And it's switched to InsertLine if a connection is drawn by
      right-clicking an item. Releasing the mouse places the connection.
      */
    enum Mode { InsertLine, MoveItem };

    DiagramScene(QObject *parent = 0);
    QMap<int, ProcessorItem*> getItemMap() const
        { return mItemMap; }

    void reset();
public slots:
    void setMode(Mode mode);

    void createProcessorItem(ProcessorInfo pinfo);
    void deleteProcessorItem(ProcessorInfo pinfo);
    void updateProcessorItem(ProcessorInfo pinfo);

    void createConnection(int source, int target);
    void deleteConnection(int source, int target);

    void setPosition(const QString& name, QPoint position);

private slots:
    void onSelectionChanged();

signals:
    void processorSelected(int id);

    void createProcessorRequested(QString name);
    void deleteProcessorRequested(int id);

    void createConnectionRequested(int source, int target);
    void deleteConnectionRequested(int source, int target);

protected:
    /*
      Starts drawing arrow by right-clicking. Inserts item by
      left-clicking.
      */
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    /*
      Opens a dialog for setting options of an item.
      */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    /*
      In MoveItem Mode moves an item. In InsertLine Mode moves
      an arrow.
      */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    /*
      Inserts an arrow between two items in InsertLine Mode.
      */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    /*
      Deletes a selected item or arrow by pressing delete.
      */
    void keyPressEvent(QKeyEvent *event);

private:
    void createDiagramItem(ProcessorItem *item);
    Mode mMode;
    QGraphicsLineItem *mDrawingLine;
    /*
      Maps all items in the scene to their id.
      */
    QMap<int, ProcessorItem*> mItemMap;
    /*
      The key of an arrow is defined by the id's
      of the start and end items.
      */
    typedef QPair<int,int> ArrowKey;
    /*
      Maps all arrows in the scene to their key.
      */
    QMap<ArrowKey, Arrow*> mArrowMap;
};

#endif // DIAGRAMSCENE_H
