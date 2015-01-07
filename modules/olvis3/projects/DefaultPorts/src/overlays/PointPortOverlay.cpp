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

#include "PointPortOverlay.h"

#include <QMouseEvent>
#include <QDebug>
#include "VideoDisplayWidget.h"

PointPortOverlay::PointPortOverlay(QString name)
    : Overlay(name), mButtonDown(false)
{
}

void PointPortOverlay::paint(QPainter &p, bool)
{
    p.setViewTransformEnabled(true);
    mTransform = p.deviceTransform();
    if (mButtonDown) {
        QPen pen(QBrush(Qt::green), 2.0);
        pen.setCosmetic(true);
        p.setPen(pen);
        p.drawLine(mCurrentPoint - QPointF(0, 5), mCurrentPoint + QPointF(0, 5));
        p.drawLine(mCurrentPoint - QPointF(5, 0), mCurrentPoint + QPointF(5, 0));
    }
}

void PointPortOverlay::setValue(const QVariant &)
{
    //mPoint = value.toPointF();
}

void PointPortOverlay::mousePressEvent(QMouseEvent *event)
{
    mCurrentPoint = closestValidPoint(event->pos());
    mButtonDown = true;
    mWidget->update();
}

void PointPortOverlay::mouseMoveEvent(QMouseEvent *event, QList<QPoint> snapPoints)
{
    if (mButtonDown) {
        mCurrentPoint = closestValidPoint(event->pos());
        mWidget->update();
    }
}

void PointPortOverlay::mouseReleaseEvent(QMouseEvent *)
{
    mButtonDown = false;
    emit valueChanged(mPortId, mCurrentPoint);
}

QPointF PointPortOverlay::closestValidPoint(QPoint p)
{
    QPointF result = mTransform.inverted().map(p);
    if (result.x() < 0)
        result.setX(0.0);
    else if (result.x() >= mWidget->imageSize().width())
        result.setX(mWidget->imageSize().width() - 1);
    if (result.y() < 0)
        result.setY(0.0);
    else if (result.y() >= mWidget->imageSize().height())
        result.setY(mWidget->imageSize().height() - 1);
    return result;
}
