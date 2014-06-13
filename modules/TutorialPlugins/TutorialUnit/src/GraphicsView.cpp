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

/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include "GraphicsView.h"
#include "Circle.h"
#include "Robot.h"
#include <SensorDataSystem.h>
#include <math.h>

GraphicsView::GraphicsView()
{
    QGraphicsScene *scene = new QGraphicsScene(0, 0, 600, 400);
    for (int i = 0; i < 4; ++i) {
        Circle *item = new Circle;
        scene->addItem(item);
        objects << item;
    }
    robot = new Robot;
    //robot->scale(1.2, 1.2);
    robot->setPos(300, 200);
    scene->addItem(robot);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setBackgroundBrush(QColor(230, 200, 167));
    setWindowTitle("Tutorial unit");
    connect(&timer, SIGNAL(timeout()), SLOT(sendImage()));
    reset(false);
    timer.start(100);
    provider = SensorSystemInterface::createProvider("tutorial/image");

}

GraphicsView::~GraphicsView()
{
    delete provider;
}

Pose2d GraphicsView::getPosition()
{
    QPointF pos = robot->pos();
    double angle = robot->rotation();
    return Pose2d(pos.x(), pos.y(), angle);
}

void GraphicsView::moveRobotAbs(QPointF position, int durationInMs)
{
    //position += robot->torso->boundingRect().topLeft(); // move the robot's center
    QRectF rect = sceneRect();
    if(position.x() < rect.left())
        position.setX(rect.left());
    if(position.x() > rect.right())
        position.setX(rect.right());
    if(position.y() < rect.top())
        position.setY(rect.top());
    if(position.y() > rect.bottom())
        position.setY(rect.bottom());

    if(durationInMs < 0)
    {
        double offset = (robot->pos() - position).manhattanLength();
        offset = offset * 2000.0 / 500.0; // a 500 pixel travel takes 2 seconds
        durationInMs = qRound(offset);
    }

    if(durationInMs > 5)
    {
        QPropertyAnimation *anim = new QPropertyAnimation(robot, "pos");
        anim->setEndValue(position);
        anim->setDuration(durationInMs);
        connect(anim, SIGNAL(finished()), SIGNAL(movementFinished()));
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        robot->setPos(position);
        emit movementFinished();
    }
}

void GraphicsView::moveRobotRel(QPointF dist, int durationInMs)
{
    moveRobotAbs(robot->pos() + dist, durationInMs);
}



void GraphicsView::rotateRobotAbs(double angle, int durationInMs)
{
    if(durationInMs < 0)
    {
        double offset = abs(angle-robot->rotation());
        offset = offset * 1000.0 / 180.0; // a 180 degree rotation takes 1 seconds
        durationInMs = qRound(offset);
    }
    if(durationInMs > 5)
    {
        QPropertyAnimation *anim = new QPropertyAnimation(robot, "rotation");
        anim->setEndValue(angle);
        anim->setDuration(durationInMs);
        connect(anim, SIGNAL(finished()), SIGNAL(movementFinished()));
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        robot->setRotation(angle);
        emit movementFinished();
    }
}

void GraphicsView::rotateRobotRel(double angle, int durationInMs)
{
    rotateRobotAbs(robot->rotation() + angle, durationInMs);
}

void GraphicsView::setGripperState(bool open)
{
    robot->setGripper(open);
    foreach(Circle* object, objects)
    {
        if(open)
        {
            if(object->parentItem() == robot)
            {
                QPointF pos = object->mapToScene(QPointF(0, 0));
                object->setParentItem(0);
                object->setPos(pos);
            }
        }
        else if(robot->lowerRightArmItem->collidesWithItem(object) || robot->lowerRightLegItem->collidesWithItem(object))
        {
            object->setParentItem(robot);
            object->setPos(70,0);
            break;
        }
    }
    emit movementFinished();
}

void GraphicsView::sendImage()
{
    emit positionUpdate(robot->pos(), robot->rotation());
    QImage img(sceneRect().size().toSize(), QImage::Format_RGB32);
    QPainter painter(&img);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    render(&painter);
    if(provider)
        provider->update(img);
}


void GraphicsView::reset(bool randomPositions)
{
    setGripperState(true);
    robot->setPos(300, 200);
    robot->setRotation(0);
    int i=0;
    foreach(Circle* object, objects)
    {
        int y = 60*i;
        int x = 40;
        if(randomPositions)
            x = 300;
        object->setPos(x,50+y);
        i++;
    }
    if(randomPositions)
    {
        srand(QDateTime::currentDateTime().toTime_t());
        foreach(Circle* object, objects)
        {
            do {
                int y = rand() % 360;
                int x = rand() % 60;
                object->setPos(20 + x,20+y);
            } while(object->collidingItems().size() > 0);
        }
    }
}
