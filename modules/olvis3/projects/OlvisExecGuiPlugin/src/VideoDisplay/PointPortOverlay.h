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

#ifndef POINTPORTOVERLAY_H
#define POINTPORTOVERLAY_H

#include "Overlay.h"

#include <QPainter>
#include <QPoint>
#include <QPointF>
#include <QTransform>

class PointPortOverlay : public Overlay {
Q_OBJECT
public:
    PointPortOverlay(QString name);

    virtual void setInitialPos(const QPoint& ) {}
    virtual void paint(QPainter& painter, bool showControls);
    virtual void setValue(const QVariant &value);

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent*  event);
    virtual void mouseMoveEvent(QMouseEvent*  event, QList<QPoint> snapPoints);

    virtual bool contains(const QPoint& ) { return false; }

private:
    QPointF closestValidPoint(QPoint p);

    QTransform mTransform;
    QPointF mPoint, mCurrentPoint;
    bool mButtonDown;
};

#endif // POINTPORTOVERLAY_H
