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
#include <QMessageBox>
#include <QInputDialog>

#include <ports/PolygonPort.h>
#include <ports/PosePort.h>
#include <math.h>

SimpleShapeOverlay::SimpleShapeOverlay(QString name)
    : DataOverlay(name)
{
    mPen.setWidthF(1.0);
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
        mTransform = painter.combinedTransform();

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
        mPoint = element.toPointF();
        int distance = 5 * mPen.widthF();

        if (mState == Idle && mActive) {
            QPen pen;
            pen.setJoinStyle(Qt::MiterJoin);
            pen.setCosmetic(true);

            // Draw bounding rect
            pen.setColor(QColor(Qt::transparent));

            QPoint topLeft = (mPoint - QPoint(distance, distance)).toPoint();
            QPoint bottomRight = (mPoint + QPoint(distance, distance)).toPoint();

            QRect boundingRect(topLeft, bottomRight);
            painter.setBrush(QColor(0, 0, 0, 160));
            painter.setPen(pen);
            painter.drawRect(boundingRect);
        }

        // Draw marker
        mPoint += QPointF(0.5, 0.5);
        painter.setPen(mPen);
        painter.drawLine(mPoint - QPointF(0, distance), mPoint + QPointF(0, distance));
        painter.drawLine(mPoint - QPointF(distance, 0), mPoint + QPointF(distance, 0));

        painter.setViewTransformEnabled(true);

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
    } else if(mPortTypeName == "Line") {
        painter.drawLine(element.toLineF());
    }
}

void SimpleShapeOverlay::setPortId(const PortId &portId, bool output)
{
    DataOverlay::setPortId(portId, output);
    mPortTypeName = mVisionInterface->getPortInfo(portId).typeName;
}

void SimpleShapeOverlay::mousePressEvent(QMouseEvent *event)
{
    if (mPortTypeName == "Point") {
        // Check descriptive text
        QPoint pos = event->pos();
        if(RectOverlay::contains(pos)){
            DataOverlay::mousePressEvent(event);
        } else {
            // Check shape
            int distance = 5 * mPen.widthF();

            QPoint topLeft = (mPoint - QPoint(distance, distance)).toPoint();
            QPoint bottomRight = (mPoint + QPoint(distance, distance)).toPoint();
            QRect rect(topLeft, bottomRight);

            QRect visibleRect = mTransform.mapRect(rect);

            if(visibleRect.contains(pos)){
                bool ok;
                double d = QInputDialog::getDouble(0, tr("Scaling"), 0, mPen.widthF(), 0, 10, 1, &ok);
                if(ok){
                    mPen.setWidthF(d);
                }
            }
        }
    } else {
        DataOverlay::mousePressEvent(event);
    }
}

bool SimpleShapeOverlay::contains(const QPoint &pos)
{
    if (mPortTypeName == "Point") {
        // Check descriptive text
        bool text = RectOverlay::contains(pos);

        // Check shape
        int distance = 5 * mPen.widthF();

        QPoint topLeft = (mPoint - QPoint(distance, distance)).toPoint();
        QPoint bottomRight = (mPoint + QPoint(distance, distance)).toPoint();
        QRect rect(topLeft, bottomRight);

        QRect visibleRect = mTransform.mapRect(rect);
        bool point = visibleRect.contains(pos);

        return text || point;
    }
    return false;
}
