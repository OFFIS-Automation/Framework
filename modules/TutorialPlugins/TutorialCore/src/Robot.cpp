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

#include <QtGui>

#include "Robot.h"



RobotTorso::RobotTorso(QGraphicsItem *parent)
: RobotPart(parent)
{
}

QRectF RobotTorso::boundingRect() const
{
    return QRectF(-30, -40, 60, 80);
}

void RobotTorso::paint(QPainter *painter,
           const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    painter->setBrush(color);
    painter->drawRoundedRect(-20, -40, 40, 80, 25, 25, Qt::RelativeSize);
//    painter->drawEllipse(-25, -20, 20, 20);
    painter->drawEllipse(5, -40, 20, 20);
//    painter->drawEllipse(-20, 22, 20, 20);
    painter->drawEllipse(5, 20, 20, 20);
}

RobotLimb::RobotLimb(QGraphicsItem *parent, QColor c)
    : RobotPart(parent)
{
    if(c.isValid())
        color = c;
}

QRectF RobotLimb::boundingRect() const
{
    return QRectF(-5, -5, 40, 10);
}

void RobotLimb::paint(QPainter *painter,
           const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(color);
    painter->drawRoundedRect(boundingRect(), 50, 50, Qt::RelativeSize);
    painter->drawEllipse(-5, -5, 10, 10);
}

Robot::Robot(QGraphicsItem *parent)
    : RobotPart(parent)
{
    setFlag(ItemHasNoContents);

    torso = new RobotTorso(this);
    upperRightArmItem = new RobotLimb(torso);
    lowerRightArmItem = new RobotLimb(upperRightArmItem, Qt::red);
    upperRightLegItem = new RobotLimb(torso);
    lowerRightLegItem = new RobotLimb(upperRightLegItem, Qt::red);
    upperRightArmItem->setPos(15, -30);
    lowerRightArmItem->setPos(30, 0);
    upperRightLegItem->setPos(15, 30);
    lowerRightLegItem->setPos(30, 0);

}

void Robot::setGripper(bool open)
{
    if(!open)
    {
        upperRightArmItem->setRotation(15.0);
        upperRightLegItem->setRotation(-15.0);
    }
    else
    {
        upperRightArmItem->setRotation(0);
        upperRightLegItem->setRotation(0);
    }
}

QRectF Robot::boundingRect() const
{
    return QRect();
}

void Robot::paint(QPainter *painter,
                  const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
//! [9]
