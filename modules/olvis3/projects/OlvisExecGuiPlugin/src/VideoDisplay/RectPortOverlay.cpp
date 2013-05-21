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

#include "RectPortOverlay.h"
#include "src/OlvisSingleton.h"
#include "VideoDisplayWidget.h"

#include <QMouseEvent>
#include <QDebug>

RectPortOverlay::RectPortOverlay(QString name)
    : RectOverlay(name)
{
    mBoundedToImage = true;
}

void RectPortOverlay::setInitialPos(const QPoint &pos)
{
    mRect = OlvisSingleton::instance().getPortValue(mPortId).toRect();
    if (mRect.isEmpty()) {
        RectOverlay::setInitialPos(pos);
        emit valueChanged(mPortId, QRectF(mRect));
    }
}

void RectPortOverlay::mouseMoveEvent(QMouseEvent *event, QList<QPoint> snapPoints)
{
    RectOverlay::mouseMoveEvent(event, snapPoints);
}

void RectPortOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    RectOverlay::mouseReleaseEvent(event);
    //if (mState != Idle)
    emit valueChanged(mPortId, QRectF(mRect));
}

void RectPortOverlay::setValue(const QVariant &value)
{
    mRect = value.toRectF().toRect();
    if(mWidget)
        mWidget->dataChanged(this);
}

void RectPortOverlay::ensureBounds()
{
    QRect startRect = mRect;
    RectOverlay::ensureBounds();
    mRect = OlvisSingleton::instance().constrainedValue(mPortId, mRect.normalized()).toRect();
    if(startRect.width() < 0) // orog rect was negative
        mRect.moveRight(startRect.topLeft().x()); // mRect is normalized, startRect is not!
    if(startRect.height() < 0)
        mRect.moveBottom(startRect.topLeft().y()); // mRect is normalized, startRect is not!
}
