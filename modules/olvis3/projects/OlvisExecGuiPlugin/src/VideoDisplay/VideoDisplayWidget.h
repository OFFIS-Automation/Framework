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

#ifndef VIDEODISPLAYWIDGET_H
#define VIDEODISPLAYWIDGET_H

#ifdef OPEN_GL
    #include <QGLWidget>
    #include <QtOpenGL>
#endif

#include <QList>
#include <QMutex>
#include <QSize>

#include <core/PortListener.h>
#include "VideoRecorder.h"

#include <opencv2/core/core.hpp>

#include <Timer.h>

#include "VideoControlToolbar.h"

class MainOverlay;
class Overlay;
class QAbstractScrollArea;
class QXmlStreamWriter;
class QXmlStreamReader;
class OlvisInterface;

class ImagePortOverlay;

struct FilterInfo;
struct PortId;

class VideoDisplayWidget :
#ifdef OPEN_GL
        public QGLWidget
#else
        public QWidget
#endif
{
    Q_OBJECT
public:
    explicit VideoDisplayWidget(QWidget *parent = 0);
    virtual ~VideoDisplayWidget();

    virtual void writeCurrentConfig(QXmlStreamWriter& writer);
    virtual void readConfig(QXmlStreamReader& reader);

    virtual void setScrollArea(QAbstractScrollArea* scrollArea);
    virtual QAbstractScrollArea* scrollArea();

    virtual void setOffset(int dx, int dy);
    virtual QSize imageSize();
    virtual double imageRate();
    virtual void paintContent(QPainter& painter, bool showControls);

    virtual void dataChanged(Overlay* source);

    static QPoint closestPoint(const QRect& bounds, QPoint point);

signals:
    void doubleClick(VideoDisplayWidget* source);
    void valueChanged(const PortId& portId, const QVariant& value);
    void newData();
    void threadSaveUpdate();

public slots:
    virtual void deleteListeners(const FilterInfo &info);
    virtual void removeOverlay(Overlay* overlay);

    virtual void clear();
    virtual void setZoomToFit(bool value);
    virtual void setZoom(double value);
    virtual void setHorizontalFlip(bool flip);
    virtual void setVerticalFlip(bool flip);
    virtual void screenshot();
    virtual void recordVideo();

    virtual void editChanged(QAction* action);
    virtual void boolChanged(QAction* action);
    virtual void choiceChanged(QAction* action);

protected:


    virtual void paintEvent(QPaintEvent* event);
    virtual QRect boundingRect();

    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);

    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

    virtual void wheelEvent(QWheelEvent* event);
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);

    // Set the widget's main overlay
    virtual void setMainOverlay(MainOverlay *overlay);
    // Add overlay to this widget
    virtual void addOverlay(Overlay* overlay);
    // Get overlay corresponding to the given port
    virtual Overlay* getOverlay(PortId portId);


protected slots:
    virtual void portValueChanged(int filterId, const QString& portId, QVariant value);
    virtual void showInfo(const QString& info);
    virtual void removeInfo();

private:
    const OlvisInterface& mModel;
    QMutex mLock;
    bool mUpdateRequested;

    bool mScaleToFit;
    bool mHorizontalFlip, mVerticalFlip;
    double mScale;
    QPoint mOffset;
    QTransform mTransform;

    double mIncrementScaler;

    QTimer mInfoTimer;
    QString mInfo;

    VideoRecorder mRecorder;

    MainOverlay* mMainOverlay;

    Overlay* mActiveOverlay;
    QList<Overlay *> mOverlays;
    QAbstractScrollArea* mScrollArea;
    QFont mFont;

    Timer mTimer;

    VideoControlToolbar* mToolbar;
    QPoint mMouseDownAt;
    QVariant mStartValue;
    bool mDragging;

    cv::Mat lastScreenShot;

    Timer mProfiling;
};

#endif // VIDEODISPLAYWIDGET_H
