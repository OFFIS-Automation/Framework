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

#include "RectOverlay.h"

#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMouseEvent>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "VideoDisplayWidget.h"
#include <QByteArray>
#include "qglobal.h"

RectOverlay::RectOverlay(QString name) : Overlay(name), mRect(),
    mFirstRun(true),
    mScaling(true),
    mFixedAspectRatio(false),
    //mAspectRatio(1.0),
    mBoundedToImage(false),
    mState(Idle),
    mPen(QBrush(Qt::black), 2.0)
{
    mPen.setJoinStyle(Qt::MiterJoin);
    mPen.setCosmetic(true);
}

RectOverlay::~RectOverlay()
{
}

void RectOverlay::writeCurrentConfig(QXmlStreamWriter &writer)
{
    Overlay::writeCurrentConfig(writer);
    writer.writeEmptyElement("rect");
    writer.writeAttribute("coords", QString("%1, %2, %3, %4").arg(mRect.x()).arg(mRect.y()).arg(mRect.width()).arg(mRect.height()));
}

void RectOverlay::readElement(QXmlStreamReader &reader)
{
    if (reader.name() == "rect") {
        QStringList p = reader.attributes().value("coords").toString().split(",");
        mRect = QRect(p[0].toInt(), p[1].toInt(), p[2].toInt(), p[3].toInt());
    } else {
        Overlay::readElement(reader);
    }
}

void RectOverlay::setInitialPos(const QPoint &pos)
{
    mRect.setRect(pos.x(), pos.y(), 200, 20);
}

void RectOverlay::paintContent(QPainter &p)
{
    mPen.setColor(Qt::black);
    p.setPen(mPen);
    if (mState == Scaling || mState == Dragging)
        p.drawRect(mOrgRect);
    else
        p.drawRect(mRect);
}

void RectOverlay::paint(QPainter& p, bool showControls)
{

    p.setViewTransformEnabled(true);
    mTransform = p.combinedTransform();
    paintContent(p);
    if (mState == Scaling || mState == Dragging) {
        // Draw frame to show selection
        p.setBrush(Qt::NoBrush);
        mPen.setColor(Qt::green);
        p.setPen(mPen);
        p.drawRect(mRect);
        drawText(p, QString("%1,%2 %3x%4").arg(mRect.x()).arg(mRect.y()).arg(mRect.width()).arg(mRect.height()));
    }

    QRect realRect;
    // Transform rect to real rect on unix
    // Using automated transformation is doing strange stuff
    // => we do it ourselfs
    p.setViewTransformEnabled(false);
    realRect = mTransform.mapRect(mRect);

    if (mState == Idle && showControls && mActive) {
        // Draw handle
        QRectF handle(-3, -3, 7, 7);
        p.setBrush(Qt::black);
        mPen.setColor(Qt::white);
        p.setPen(mPen);
        p.drawRect(handle.translated(realRect.bottomRight()));
        // Draw cancel / delete button
        p.drawPixmap(realRect.topLeft() - QPoint(7, 7), QPixmap(":/olvisExecGui/cancel.png"));
        if (showClear())
            p.drawPixmap(realRect.topLeft() + QPoint(9, -7), QPixmap(":/olvisExecGui/delete.png"));
    }

}

//void RectOverlay::setAspectRatio(double aspectRatio)
//{
//    if (mAspectRatio != aspectRatio)
//        mRect.setHeight(mRect.width() / aspectRatio);
//    mAspectRatio = aspectRatio;
//}

void RectOverlay::ensureRectSize(int minWidth, int minHeight)
{
    if (mRect.width() >= 0 && mRect.width() < minWidth)
        mRect.setWidth(minWidth);
    if (mRect.height() >= 0 && mRect.height() < minHeight)
        mRect.setHeight(minHeight);
    if (mRect.width() < 0 && -mRect.width() < minWidth)
        mRect.setWidth(-minWidth);
    if (mRect.height() < 0 && -mRect.height() < minHeight)
        mRect.setHeight(-minHeight);
}

void RectOverlay::ensureAspectRatio()
{
    QSize newSize = mOrgRect.size();
    newSize.scale(mRect.size(), Qt::KeepAspectRatio);
    mRect.setSize(newSize);
    /*if (mRect.height() * mAspectRatio < mRect.width())
        mRect.setHeight(mRect.width() / mAspectRatio);
    else
        mRect.setWidth(mRect.height() * mAspectRatio);*/
}

void RectOverlay::ensureBounds()
{
    if (mState == Scaling) {
        mRect = mRect.intersected(QRect(QPoint(0,0), mWidget->imageSize()));
    } else if (mState == Dragging) {
        if (mRect.left() < 0)
            mRect.moveLeft(0);
        if (mRect.top() < 0)
            mRect.moveTop(0);
        if (mRect.right() > mWidget->imageSize().width() - 1)
            mRect.moveRight(mWidget->imageSize().width() - 1);
        if (mRect.bottom() > mWidget->imageSize().height() - 1)
            mRect.moveBottom(mWidget->imageSize().height() - 1);
    }
}

QRect RectOverlay::boundingRect()
{
    return mRect;
}

bool RectOverlay::contains(const QPoint &pos)
{
    QRect visibleRect = getVisibleRect();
    return  visibleRect.contains(pos);
}

QList<QPoint> RectOverlay::snapPoints()
{
    QList<QPoint> points;
    if (mState == Scaling || mState == Dragging) {
        points.append(mOrgRect.topLeft());
        points.append(mOrgRect.bottomRight() + QPoint(1,1));
    } else {
        points.append(mRect.topLeft());
        points.append(mRect.bottomRight() + QPoint(1,1));
    }
    return points;
}

void RectOverlay::mousePressEvent(QMouseEvent *event)
{
    QRect visibleRect = getVisibleRect();
    QPoint pos = mTransform.inverted().map(event->pos());
    if (mActive) {
        if (equals(event->pos(), visibleRect.topLeft(), 8)) {
            emit removeClicked(this);
        } else if (showClear() && equals(event->pos(), visibleRect.topLeft() + QPoint(16, 0), 8)) {
            emit clearClicked();
        } else if (equals(event->pos(), visibleRect.bottomRight(), 3)) {
            mState = Scaling;
            mOrgRect = mRect;
        } else if (contains(event->pos())) {
            mState = Dragging;
            mOrgRect = mRect;
            mDragStartPos = pos;
            mWidget->setCursor(Qt::ClosedHandCursor);
        } else {
            event->ignore();
        }
    } else if (QRect(QPoint(0, 0), mWidget->imageSize()).contains(pos)) {
        mOrgRect = QRect(pos, QSize(0, 0));
        mState = Scaling;
    }
}

void RectOverlay::mouseReleaseEvent(QMouseEvent *)
{
    if (mActive)
        mWidget->setCursor(Qt::OpenHandCursor);
    mState = Idle;
}

void RectOverlay::mouseMoveEvent(QMouseEvent *event, QList<QPoint> snapPoints)
{
    QRect visibleRect = getVisibleRect();
    QPoint pos = mTransform.inverted().map(event->pos());
    if (mState == Idle) {
        if (mActive) {
            if (equals(event->pos(), visibleRect.topLeft(), 8)) {
                mWidget->setCursor(Qt::ArrowCursor);
            }  else if (showClear() && equals(event->pos(), visibleRect.topLeft() + QPoint(16, 0), 8)) {
                mWidget->setCursor(Qt::ArrowCursor);
            } else if (equals(event->pos(), visibleRect.bottomRight(), 3)) {
                mWidget->setCursor(Qt::SizeFDiagCursor);
            } else if (contains(event->pos())) {
                mWidget->setCursor(Qt::OpenHandCursor);
            } else {
                mWidget->unsetCursor();
            }
        }
        return;
    } if (mState == Scaling) {
        foreach(QPoint p, snapPoints) {
            if (qAbs(p.x() - 1 - pos.x()) < 8)
                pos.setX(p.x() - 1);
            if (qAbs(p.y() - 1 - pos.y()) < 8)
                pos.setY(p.y() - 1);
        }

        mRect.setTopLeft(mOrgRect.topLeft());
        mRect.setBottomRight(pos);
        ensureRectSize(10, 10);
        if (mBoundedToImage)
            ensureBounds();
        if (mFixedAspectRatio || (event->modifiers() & Qt::ShiftModifier)) {
            ensureAspectRatio();
            if (pos.x() < mOrgRect.left())
                mRect.moveRight(mOrgRect.left());
            if (pos.y() < mOrgRect.top())
                mRect.moveBottom(mOrgRect.top());
        }
        mRect = mRect.normalized();
    } else {
        QPoint dist = pos - mDragStartPos;
        mRect = mOrgRect;
        mRect.translate(dist);
        QPoint topLeft = mRect.topLeft();
        QPoint bottomRight = mRect.bottomRight();
        foreach(QPoint p, snapPoints) {
            if (qAbs(p.x() - topLeft.x()) < 8)
                mRect.moveLeft(p.x());
            else if (qAbs(p.x() - 1 - bottomRight.x()) < 8)
                mRect.moveRight(p.x() - 1);
            if (qAbs(p.y() - topLeft.y()) < 8)
                mRect.moveTop(p.y());
            else if (qAbs(p.y() - 1 - bottomRight.y()) < 8)
                mRect.moveBottom(p.y() - 1);
        }
        if (mBoundedToImage)
            ensureBounds();
    }
    mWidget->update();
}

void RectOverlay::setScaling(bool scaling)
{
    mScaling = scaling;
}

bool RectOverlay::equals(const QPoint &a, const QPoint &b, int dist)
{
    return (abs(a.x() - b.x()) <= dist) && (abs(a.y() - b.y()) <= dist);
}

void RectOverlay::drawText(QPainter &painter, QString text, int flags)
{
    painter.setViewTransformEnabled(false);
    painter.drawText(getVisibleRect(), flags, text);
    painter.setViewTransformEnabled(true);
}

QRect RectOverlay::getVisibleRect()
{
    QRect visibleRect = mTransform.mapRect(mRect);
    return visibleRect;
}
