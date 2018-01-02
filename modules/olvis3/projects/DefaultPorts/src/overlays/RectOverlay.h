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

#ifndef RECTOVERLAY_H
#define RECTOVERLAY_H

#include <QPen>
#include <QRect>
#include <QTransform>
//#include "OverlayFactory.h"
#include "Overlay.h"

class QPainter;
class VideoDisplayWidget;
class QMouseEvent;
class QXmlStreamReader;
class QXmlStreamWriter;

class RectOverlay : public Overlay
{
    Q_OBJECT
public:
    RectOverlay(QString name);
    virtual ~RectOverlay();
    virtual bool showClear() { return false; }
    virtual void writeCurrentConfig(QXmlStreamWriter& writer);
    virtual void readElement(QXmlStreamReader& reader);

    virtual void setInitialPos(const QPoint &pos);
    virtual void paintContent(QPainter& painter);
    virtual void paint(QPainter& painter, bool showControls);

    virtual void setScaling(bool scaling);
    virtual bool contains(const QPoint &pos);
    virtual QList<QPoint> snapPoints();

    //virtual void setAspectRatio(double aspectRatio);
    virtual QRect boundingRect();

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent*  event);
    virtual void mouseMoveEvent(QMouseEvent*  event, QList<QPoint> snapPoints);

    // helper for drawing text without viewport effects
    virtual void drawText(QPainter& painter, QString text, int flags = Qt::AlignCenter);
    virtual QRect getVisibleRect();
signals:
    void clearClicked();
protected:
    virtual bool equals(const QPoint& a, const QPoint& b, int dist);
    virtual void ensureRectSize(int minWidth, int minHeight);
    virtual void ensureAspectRatio();
    virtual void ensureBounds();

    typedef enum {
        Idle, Dragging, Scaling
    } State;

    QRect mRect, mOrgRect;
    QTransform mTransform;
    bool mFirstRun;

    bool mScaling;
    bool mFixedAspectRatio;
    bool mBoundedToImage;
    //double mAspectRatio;
    State mState;

    QPoint mDragStartPos;
    QPen mPen;
};

#endif // RECTOVERLAY_H
