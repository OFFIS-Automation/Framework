// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
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

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

#include <rc/Pose2d.h>
class Robot;
class Circle;
class DataProvider;

#include <QTimer>

#include <QtCore/qglobal.h>
#include <QObject>

#if defined(TUTORIAL_LIBRARY)
#  define TUTORIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TUTORIALSHARED_EXPORT Q_DECL_IMPORT
#endif

class TUTORIALSHARED_EXPORT GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView();
    static GraphicsView* instance();
    ~GraphicsView();
    Pose2d getPosition();
signals:
    void movementFinished();
    void positionUpdate(QPointF pos, qreal rot);
    void newImage(const QImage& img);
public slots:
    void reset(bool randomPositions);
    void moveRobotAbs(QPointF position, int durationInMs);
    void moveRobotRel(QPointF dist, int durationInMs);
    void rotateRobotAbs(double angle, int durationInMs);
    void rotateRobotRel(double angle, int durationInMs);
    void setGripperState(bool open);
protected slots:
    void sendImage();
protected:
    static GraphicsView* sInstance;
    Robot *robot;
    QList <Circle*> objects;
    QTimer timer;
};


#endif // GRAPHICSVIEW_H
