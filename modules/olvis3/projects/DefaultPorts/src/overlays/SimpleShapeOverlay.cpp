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

#include "SimpleShapeOverlay.h"

#include <QList>

#include <ports/PolygonPort.h>
#include <ports/PosePort.h>
#include <math.h>

SimpleShapeOverlay::SimpleShapeOverlay(QString name)
    : DataOverlay(name)
{
}

void SimpleShapeOverlay::paint(QPainter &painter, bool showControls)
{
    DataOverlay::paint(painter, showControls);
    if (mLastValue.isValid()) {
        QList<QVariant> rects;
        mMutex.lock();
        if (mLastValue.type() == QVariant::List)
            rects.append(mLastValue.toList());
        else
            rects.append(mLastValue);
        mMutex.unlock();
        painter.setViewTransformEnabled(true);
        painter.setPen(mPen);
        foreach (const QVariant& v, rects) {
            paintElement(painter, v);
        }
        painter.setViewTransformEnabled(false);
    }
}

void SimpleShapeOverlay::paintLegendSymbol(QPainter &painter, const QRect &rect)
{
    painter.setPen(mPen);
    QPoint point = rect.center();
    painter.drawLine(point - QPoint(0, 5), point + QPoint(0, 5));
    painter.drawLine(point - QPoint(5, 0), point + QPoint(5, 0));
}

void SimpleShapeOverlay::paintElement(QPainter &painter, const QVariant &element)
{
    if (mPortTypeName == "Rect") {
        painter.drawRect(element.toRectF().translated(QPointF(0.5, 0.5)));
    } else if (mPortTypeName == "Point") {
        QPointF point = element.toPointF();
        //drawText(painter, QString("(%1,%2)").arg(point.x()).arg(point.y()));
        point += QPointF(0.5, 0.5);
        painter.drawLine(point - QPointF(0, 5), point + QPointF(0, 5));
        painter.drawLine(point - QPointF(5, 0), point + QPointF(5, 0));
    } else if (mPortTypeName == "Polygon") {
        QPolygonF poly = port::Polygon::fromVariant(element).translated(QPointF(0.5, 0.5));
        if(poly.isClosed()){
            painter.drawPolygon(poly);
        } else {
            painter.drawPolyline(poly);
        }
    } else if (mPortTypeName == "Pose2d") {
        Pose2d pose = port::Pose2d::fromVariant(element);
        QPointF point = QPoint(pose.x, pose.y) + QPointF(0.5, 0.5);
        QPointF head = point + QPointF(10 * cos(pose.phi), 10 * sin(pose.phi));
        painter.drawLine(point - QPointF(0, 5), point + QPointF(0, 5));
        painter.drawLine(point - QPointF(5, 0), point + QPointF(5, 0));
        painter.drawLine(point, head);
    } else if(mPortTypeName == "Line")
    {
        painter.drawLine(element.toLineF());
    }
}

void SimpleShapeOverlay::setPortId(const PortId &portId, bool output)
{
    DataOverlay::setPortId(portId, output);
    mPortTypeName = mVisionInterface->getPortInfo(portId).typeName;
}

