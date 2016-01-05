// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#include "VideoDisplayWidget.h"

#include "../../../OlvisGuiPlugin/src/PluginContainer.h"
#include "src/OlvisSingleton.h"

#include "VideoControlToolbar.h"

#include <core/FilterInfo.h>
#include <QScrollArea>

#include <ports/ImagePort.h>
#include <ports/FilePort.h>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QMutexLocker>
#include <QAction>
#include <QMimeData>

#include <QDebug>
#include <QPalette>
#include <QMessageBox>

#include <QFileDialog>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QGraphicsDropShadowEffect>

VideoDisplayWidget::VideoDisplayWidget(QWidget *parent) :
#ifdef OPEN_GL
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
#else
    QWidget(parent),
#endif
    mModel(OlvisSingleton::instance()), mScaleToFit(false), mHorizontalFlip(false), mVerticalFlip(false), mScale(1.0), mOffset(0, 0),
    mIncrementScaler(1.0), mRecorder(this), mMainOverlay(0), mActiveOverlay(0), mFont(font()), mNumberDragInput(NoNumberDrag)
{
    //setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAutoFillBackground(false);
    setMouseTracking(true);
    setAcceptDrops(true);

    connect(&mModel, SIGNAL(filterDeleted(FilterInfo)), this, SLOT(deleteListeners(FilterInfo)));
    connect(this, SIGNAL(valueChanged(PortId,QVariant)), &mModel, SLOT(setPortValue(PortId,QVariant)));
    connect(&mModel, SIGNAL(portValueChanged(int,QString,QVariant)), this, SLOT(portValueChanged(int,QString,QVariant)));


    mFont.setPixelSize(20);

    mToolbar = new VideoControlToolbar(this);
    mToolbar->setVisible(false);
    mToolbar->setEnabled(false);
    mToolbar->setGeometry(QRect(QPoint(10, 10), mToolbar->sizeHint()));
    mToolbar->setAutoFillBackground(true);
    connect(mToolbar, SIGNAL(clearClicked()), this, SLOT(clear()));
    connect(mToolbar, SIGNAL(zoomChanged(double)), this, SLOT(setZoom(double)));
    connect(mToolbar, SIGNAL(zoomToFitChanged(bool)), this, SLOT(setZoomToFit(bool)));
    connect(mToolbar, SIGNAL(screenshotRequested()), this, SLOT(screenshot()));
    connect(mToolbar, SIGNAL(videoRequested()), this, SLOT(recordVideo()));
    connect(mToolbar, SIGNAL(editChangeRequested(QAction*)), this, SLOT(editChanged(QAction*)));
    connect(mToolbar, SIGNAL(boolChangeRequested(QAction*)), this, SLOT(boolChanged(QAction*)));
    connect(mToolbar, SIGNAL(choiceChangeRequested(QAction*)), this, SLOT(choiceChanged(QAction*)));
    connect(mToolbar, SIGNAL(horizontalFlipChanged(bool)), this, SLOT(setHorizontalFlip(bool)));
    connect(mToolbar, SIGNAL(verticalFlipChanged(bool)), this, SLOT(setVerticalFlip(bool)));

    connect(this, SIGNAL(newData()), &mRecorder, SLOT(dataChanged()), Qt::QueuedConnection);
    connect(this, SIGNAL(threadSaveUpdate()), this, SLOT(update()));

    connect(&mInfoTimer, SIGNAL(timeout()), this, SLOT(removeInfo()));
}

VideoDisplayWidget::~VideoDisplayWidget()
{
    qDeleteAll(mOverlays);
    if (mMainOverlay)
        delete mMainOverlay;
    delete mToolbar;
}

void VideoDisplayWidget::clear()
{
    mActiveOverlay = 0;
    qDeleteAll(mOverlays);
    mOverlays.clear();
    mToolbar->clear();
    mToolbar->setEnabled(false);

    if (mMainOverlay) {
        delete mMainOverlay;
        mMainOverlay = 0;
    }

    mScale = 1;
    update();
}

void VideoDisplayWidget::writeCurrentConfig(QXmlStreamWriter& writer)
{
    writer.writeEmptyElement("scale");
    writer.writeAttribute("value", QString::number(mScale));
    writer.writeAttribute("auto", QString::number(mScaleToFit));
    writer.writeEmptyElement("flip");
    writer.writeAttribute("horizontal", QString::number(mHorizontalFlip));
    writer.writeAttribute("vertical", QString::number(mVerticalFlip));
    if (mMainOverlay) {
        writer.writeStartElement(mMainOverlay->name());
        mMainOverlay->writeCurrentConfig(writer);
        writer.writeEndElement();

        foreach (OverlayInterface* overlay, mOverlays) {
            writer.writeStartElement(overlay->name());
            overlay->writeCurrentConfig(writer);
            writer.writeEndElement();
        }
    }
}

void VideoDisplayWidget::readConfig(QXmlStreamReader& reader)
{
    clear();
    while (reader.readNextStartElement()) {
        if (reader.name() == "scale") {
            mScale = reader.attributes().value("value").toString().toDouble();
            mScaleToFit = reader.attributes().value("auto").toString().toInt() != 0;
            mToolbar->setZoomToFit(mScaleToFit);
        } else if (reader.name() == "flip") {
            mVerticalFlip = reader.attributes().value("vertical").toString().toInt() != 0;
            mHorizontalFlip = reader.attributes().value("horizontal").toString().toInt() != 0;
            mToolbar->setFlip(mHorizontalFlip, mVerticalFlip);
        } else {
            bool isOutput = reader.attributes().value("output").toInt() == 1;
            OverlayInterface* overlay = PluginContainer::getInstance().overlayFor(reader.name().toString(), isOutput, mMainOverlay == 0, &OlvisSingleton::instance());
            if (overlay != 0) {
                overlay->setParent(this);
                overlay->readConfig(reader);
                if(!mMainOverlay)
                {
                    setMainOverlay(overlay);
                }
                else
                    addOverlay(overlay);
            }
        }
        reader.readElementText();
    }

    update();
}

void VideoDisplayWidget::deleteListeners(const FilterInfo &info)
{
    if (!mMainOverlay)
        return;
    if (mMainOverlay->portId().filter == info.id) {
        clear();
    } else {
        foreach (OverlayInterface* overlay, mOverlays) {
            if (overlay->portId().filter == info.id) {
                removeOverlay(overlay);
            }
        }
    }
}

void VideoDisplayWidget::setScrollArea(QAbstractScrollArea *scrollArea)
{
    mScrollArea = scrollArea;
}

QAbstractScrollArea* VideoDisplayWidget::scrollArea()
{
    return mScrollArea;
}

void VideoDisplayWidget::setOffset(int dx, int dy)
{
    mOffset = QPoint(dx, dy);
}

void VideoDisplayWidget::dataChanged(OverlayInterface *source)
{
    if (source == mMainOverlay)
        emit newData();
    if (mTimer.elapsedSeconds() > 0.03) {
        mTimer.restart();
        emit threadSaveUpdate();
    }
}

QRect VideoDisplayWidget::boundingRect()
{
    if (!mMainOverlay)
        return QRect();

    QSize imageSize = mMainOverlay->boundingRect().size();
    QRect bounds(QPoint(0, 0), imageSize);
    foreach (OverlayInterface* overlay, mOverlays) {
        bounds = bounds.united(overlay->boundingRect());
    }
    return bounds;
}

void VideoDisplayWidget::paintContent(QPainter &p, bool showControls)
{
    if (!mMainOverlay)
        return;

    QMutexLocker lock(&mLock);
    mMainOverlay->paint(p, showControls);

    for (int i = mOverlays.count() - 1; i >= 0; i--) {
        mOverlays.at(i)->paint(p, showControls);
    }
}

#ifdef OPEN_GL
void setupViewport(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
#ifndef OPENGL_ES
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-0.5, +0.5, -0.5, 0.5, 8.0, 15.0);

    glMatrixMode(GL_MODELVIEW);
#endif
}
#endif

void VideoDisplayWidget::showInfo(const QString &info)
{
    mInfo = info;
    mToolbar->hide();
    mInfoTimer.start(500);
    update();
}

void VideoDisplayWidget::removeInfo()
{
    mInfo = "";
    if (underMouse())
        mToolbar->show();
    update();
}


void VideoDisplayWidget::paintEvent(QPaintEvent*)
{

    static int rotation = 1;
    rotation = (rotation + 5) % 360;
#ifdef OPEN_GL
    makeCurrent();
#ifndef OPENGL_ES
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
#endif
    qglClearColor(Qt::darkGray);
    /*glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);*/

    setupViewport(width(), height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(0.0, 1.0, 0.0, 0.0);
    glRotatef(rotation, 0.0, 1.0, 0.0);
    glRotatef(0.0, 0.0, 0.0, 1.0);

    logo->draw();

    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 1);

    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);*/
#ifndef OPENGL_ES
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();/**/
#endif
    QPainter p(this);
#else
    QPainter p(this);
//    int side = qMin(width(), height());
//    p.setViewport(0, 0, side, side);

#endif

    if (!mMainOverlay)
        return;

    // calculate bounding rect
    QRect bounds = boundingRect();
    if (bounds.isEmpty())
        return;
    QSize viewportSize = bounds.size() * mScale;
    if (mScaleToFit)
        viewportSize.scale(rect().size(), Qt::KeepAspectRatio);

    if (viewportSize.width() / bounds.width() < 3.9)
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.setRenderHint(QPainter::Antialiasing, true);

    QRect viewport(QPoint(0, 0), viewportSize);
    if (mHorizontalFlip)
        viewport.setWidth(-viewportSize.width());
    if (mVerticalFlip)
        viewport.setHeight(-viewportSize.height());
    viewport.moveCenter(rect().center() - mOffset);
    p.setViewport(viewport);
    p.setWindow(bounds);

    // update scrollbars if necessary
    QSize border = (viewportSize - rect().size()).expandedTo(QSize(0, 0));
    mScrollArea->horizontalScrollBar()->setPageStep(rect().width());
    mScrollArea->horizontalScrollBar()->setRange(-border.width() / 2, border.width() / 2);
    mScrollArea->verticalScrollBar()->setPageStep(rect().height());
    mScrollArea->verticalScrollBar()->setRange(-border.height() / 2, border.height() / 2);

    mTransform = p.combinedTransform();
    paintContent(p, underMouse() && mToolbar->currentAction() == 0);

    if (mRecorder.recording()) {
        p.setFont(mFont);
        p.fillRect(rect(), QColor(0, 0, 0, 127));
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, tr("Recording started. Click here to stop."));
    } else {
        p.setFont(mFont);
        p.setPen(Qt::white);
        if (!mInfo.isEmpty()) {
            QRect r = rect();
            r.setHeight(40);
            r.moveTopLeft(QPoint(0, 0));
            p.fillRect(r, QColor(0, 0, 0, 127));
            p.drawText(r, Qt::AlignCenter, mInfo);
        }
        if (mToolbar->showImageInfo()) {
            QRect r(0, 0, 200, 50);
            r.moveBottomLeft(rect().bottomLeft());
            p.setFont(mFont);
            QSize size = imageSize();
            p.fillRect(r, QColor(0, 0, 0, 127));
            p.drawText(r, Qt::AlignLeft, tr("Size: %1 x %2\nRate: %3", "rate is update rate (Hz)").arg(size.width()).arg(size.height()).arg(1000.0/imageRate()));
        }
    }
}

QSize VideoDisplayWidget::imageSize()
{
    if (mMainOverlay)
        return mMainOverlay->boundingRect().size();
    return QSize();
}

double VideoDisplayWidget::imageRate()
{
    // TODO
    return 0.0;
}

void VideoDisplayWidget::portValueChanged(int filterId, const QString& portId, QVariant value)
{
    PortId pId;
    pId.filter = filterId;
    pId.port = portId;
    OverlayInterface* overlay = getOverlay(pId);
    if (overlay) {
        overlay->setValue(value);
        //update();
    }
}

void VideoDisplayWidget::dropEvent(QDropEvent* event)
{
    if (event->mimeData()->hasFormat("application/x-olvis-port"))
    {
        QList<QByteArray> parts = event->mimeData()->data("application/x-olvis-port").split('#');
        PortId portId(parts[1].toInt(), parts[2]);
        //const OlvisInterface& model = OlvisSingleton::instance();
        //int type = parts[3].toInt();
        PortInfo info = OlvisSingleton::instance().getPortInfo(portId);
        OverlayInterface* overlay = PluginContainer::getInstance().overlayFor(info, parts[0] == "output", mMainOverlay == 0, &OlvisSingleton::instance());
        if(!overlay)
            return;
        overlay->setPortId(portId, parts[0] == "output");
        overlay->setParent(this);
        if(!mMainOverlay)
        {
            setMainOverlay(overlay);
            QList<PortInfo> ports = OlvisSingleton::instance().getFilter(overlay->portId().filter).typeInfo.outputs;
            foreach (PortInfo p, ports) {
                if(p.typeName == "PhysicalSize")
                {
                    PortInfo info = OlvisSingleton::instance().getPortInfo(PortId(overlay->portId().filter, p.name));
                    OverlayInterface* scaleOverlay = PluginContainer::getInstance().overlayFor(info,true, false, &OlvisSingleton::instance());
                    scaleOverlay->setParent(this);
                    scaleOverlay->setPortId(PortId(portId.filter, info.name), true);
                    addOverlay(scaleOverlay);
                    scaleOverlay->setInitialPos(QPoint(15,15));
                    mToolbar->addPortAction(PortId(portId.filter, info.name));
                }
            }
        }
        else
        {
            foreach (OverlayInterface* o, mOverlays) {
                if (o->portId() == portId)
                    removeOverlay(o);
            }
            addOverlay(overlay);
            overlay->setInitialPos(mTransform.inverted().map(event->pos()));
            if(overlay->name() == "ScaleBarOverlay")
                mToolbar->addPortAction(PortId(portId.filter, info.name));
        }
    }
    else if (event->mimeData()->hasFormat("application/x-sensorSystem-value"))
    {
        if(!mMainOverlay)
            return;
        OverlayInterface* overlay = PluginContainer::getInstance().overlayFor("SensorSystemOverlay", true, false, &OlvisSingleton::instance());
        if(overlay)
        {
            QString sensorId = event->mimeData()->data("application/x-sensorSystem-value");
            overlay->setPortId(PortId(0, sensorId), true);
            overlay->setParent(this);
            addOverlay(overlay);
            overlay->setInitialPos(mTransform.inverted().map(event->pos()));
        }
    }
    update();
}

void VideoDisplayWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-olvis-port"))
    {
        QList<QByteArray> parts = event->mimeData()->data("application/x-olvis-port").split('#');
        if (parts.size() != 4) {
            qCritical() << "mimetype application/x-olvis-port not used according to specification";
            return;
        }
        PortId portId(parts[1].toInt(), parts[2]);
        PortInfo info = OlvisSingleton::instance().getPortInfo(portId);
        OverlayInterface* overlay = PluginContainer::getInstance().overlayFor(info, parts[0] == "output", mMainOverlay == 0, &OlvisSingleton::instance());
        if (overlay) {
            delete overlay;
            event->acceptProposedAction();
        }
    }
    else if (event->mimeData()->hasFormat("application/x-sensorSystem-value"))
    {
        if(mMainOverlay)
            event->acceptProposedAction();
    }
}

void VideoDisplayWidget::setMainOverlay(OverlayInterface *overlay)
{
    // Set main overlay
    mMainOverlay = overlay;
    connect(mMainOverlay, SIGNAL(updated(OverlayInterface*)), SLOT(dataChanged(OverlayInterface*)));

    // Add all input ports of the filter to the toolbar
    int filterId = mMainOverlay->portId().filter;
    QList<PortInfo> ports = OlvisSingleton::instance().getFilter(filterId).typeInfo.inputs;
    foreach (PortInfo p, ports) {
        if(p.constraints.value("displayVisibility", true).toBool())
            mToolbar->addPortAction(PortId(filterId, p.name));
    }
    // Enable the toolbar
    mToolbar->setEnabled(true);
}

void VideoDisplayWidget::removeOverlay(OverlayInterface *overlay)
{
    if (overlay == mActiveOverlay)
        mActiveOverlay = 0;

    // Remove action from toolbar, if it is not an input of the main overlay
    if (!overlay->isOutput())
    {
        PortId id = overlay->portId();
        if(id.filter == mMainOverlay->portId().filter) // this port belongs to the same filter as the main image
        {
            PortInfo info = OlvisSingleton::instance().getPortInfo(id);
            // check the user hint on displaying it ...
            if(!info.constraints.value("displayVisibility", true).toBool())
                mToolbar->removePortAction(id);
        }
        else // this is form a different filter, remove it
            mToolbar->removePortAction(id);
    }
    // Remove the overlay
    mOverlays.removeAll(overlay);
    delete overlay;

    update();
}

void VideoDisplayWidget::addOverlay(OverlayInterface *overlay)
{
    // Add the overlay
    connect(overlay, SIGNAL(removeOverlay(OverlayInterface*)), SLOT(removeOverlay(OverlayInterface*)));
    connect(mMainOverlay, SIGNAL(updated(OverlayInterface*)), SLOT(dataChanged(OverlayInterface*)));
    mOverlays.append(overlay);

    // Add action to toolbar, if input overlay
    if (!overlay->isOutput())
        mToolbar->addPortAction(overlay->portId());
}

OverlayInterface *VideoDisplayWidget::getOverlay(PortId portId)
{
    foreach (OverlayInterface* overlay, mOverlays) {
        if (overlay->portId() == portId)
            return overlay;
    }
    return 0;
}

OverlayInterface *VideoDisplayWidget::getOverlay(PortId portId, bool isOutput)
{
    foreach (OverlayInterface* overlay, mOverlays) {
        if (overlay->portId() == portId && overlay->isOutput() == isOutput)
            return overlay;
    }
    return 0;
}

void VideoDisplayWidget::editChanged(QAction*)
{
    if (mActiveOverlay) {
        mActiveOverlay->setActive(false);
        mActiveOverlay = 0;
    }
    PortInfo p = mToolbar->currentPortInfo();
    if (p.typeName == "Rect" || p.typeName == "Point")
        setCursor(Qt::CrossCursor);
    else if (p.typeName == "Integer" || p.typeName == "Real") {
        mIncrementScaler = p.constraints.value("sensitivity", 1.0).toDouble();
        setCursor(Qt::SplitHCursor);
    } else
        setCursor(Qt::ArrowCursor);
}

void VideoDisplayWidget::boolChanged(QAction *action)
{
    showInfo(QString("%1: %2").arg(mToolbar->portId(action).port).arg(action->isChecked()));
    emit valueChanged(mToolbar->portId(action), QVariant(action->isChecked()));
}

void VideoDisplayWidget::choiceChanged(QAction *action)
{
    showInfo(QString("%1: %2").arg(mToolbar->portId(action).port).arg(action->text()));
    emit valueChanged(mToolbar->portId(action), action->data());
}
void VideoDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    if (mRecorder.recording()) {
        mToolbar->setVisible(true);
        mRecorder.finishVideo();
    } else if (mToolbar->currentAction()) {
        // first, check if there is an overlay attached

        if (mToolbar->currentPortInfo().typeName == "PhysicalSize")
        {
            OverlayInterface* overlay = getOverlay(mToolbar->currentPortId(), false);
            if(!overlay)
            {
                overlay = PluginContainer::getInstance().overlayFor("MeasureOverlay", false, false, &OlvisSingleton::instance());
                overlay->setParent(this);
                addOverlay(overlay);
                overlay->setPortId(mToolbar->currentPortId(), false);
            }
            if(overlay)
                overlay->mousePressEvent(event);
        }
        else if (mToolbar->currentPortInfo().typeName == "Integer" || mToolbar->currentPortInfo().typeName == "Real") {
            mStartValue = OlvisSingleton::instance().getPortValue(mToolbar->currentPortId());
            mMouseDownAt = event->pos();
            if(event->button() == Qt::LeftButton)
                mNumberDragInput = CoarseNumberDrag;
            else
                mNumberDragInput = FineNumberDrag;
        } else if (mToolbar->currentPortInfo().typeName == "Rect" || mToolbar->currentPortInfo().typeName == "Point") {
            OverlayInterface* overlay = getOverlay(mToolbar->currentPortId());
            PortInfo info;
            if (overlay == 0) {
                info = OlvisSingleton::instance().getPortInfo(mToolbar->currentPortId());
                overlay = PluginContainer::getInstance().overlayFor(info, false, false, &OlvisSingleton::instance());
                if(overlay)
                {
                    overlay->setPortId(mToolbar->currentPortId(), false);
                    overlay->setParent(this);
                    addOverlay(overlay);
                }
            }
            overlay->mousePressEvent(event);
        }

    } else {

        if (mActiveOverlay) {
            mActiveOverlay->mousePressEvent(event);
            if (event->isAccepted())
                return;
            mActiveOverlay->setActive(false);
            mActiveOverlay = 0;
        }

        foreach(OverlayInterface* overlay, mOverlays) {
            if (overlay->contains(event->pos())) {
                mActiveOverlay = overlay;
                overlay->setActive(true);
                mOverlays.removeAll(overlay);
                mOverlays.prepend(overlay);
                break;
            }
        }

        update();
    }
}

void VideoDisplayWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mNumberDragInput = NoNumberDrag;
    if (mToolbar->currentAction()) {
        PortInfo p = mToolbar->currentPortInfo();
        OverlayInterface* overlay = getOverlay(mToolbar->currentPortId(), false);
        if (overlay && p.typeName == "PhysicalSize")
        {
            overlay->mouseReleaseEvent(event);
        }
        else if ((p.typeName == "Rect" || p.typeName == "Point") && overlay)
            overlay->mouseReleaseEvent(event);
    } else if (mActiveOverlay != 0)
        mActiveOverlay->mouseReleaseEvent(event);
    update();

}

void VideoDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    OverlayInterface* overlay = mActiveOverlay;
    if (mToolbar->currentAction()) {
        PortInfo p = mToolbar->currentPortInfo();
        if(p.typeName == "PhysicalSize")
        {
            overlay = getOverlay(mToolbar->currentPortId(), false);
        }
        else if (p.typeName == "Rect" || p.typeName == "Point") {
            overlay = getOverlay(mToolbar->currentPortId());
        } else if (mNumberDragInput != NoNumberDrag) {
            PortId portId = mToolbar->currentPortId();
            PortInfo pInfo = mModel.getPortInfo(portId);
            double range = pInfo.constraints.value("max").toFloat() - pInfo.constraints.value("min").toFloat();
            range = range * mIncrementScaler;
            if(mNumberDragInput == FineNumberDrag)
                range *= 0.1;
            int dist = mMouseDownAt.x() - event->pos().x();
            QVariant newValue;
            if (p.typeName == "Integer") {
                newValue = mStartValue.toInt() - (int) (dist * range / size().width());
            } else if (p.typeName == "Real") {
                newValue = mStartValue.toDouble() - dist * range / size().width();
            }
            newValue = mModel.constrainedValue(portId, newValue);
            showInfo(QString("%2: %1").arg(newValue.toString()).arg(p.desc));
            emit valueChanged(portId, newValue);
        }
    }
    if (overlay != 0) {
        QList<QPoint> snapPoints;
        if ((event->modifiers() & Qt::AltModifier) == 0) {
            snapPoints.append(mMainOverlay->snapPoints());
            foreach (OverlayInterface* overlay, mOverlays) {
                snapPoints.append(overlay->snapPoints());
            }
        }
        overlay->mouseMoveEvent(event, snapPoints);
    }
}

void VideoDisplayWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClick(this);
    event->accept();
}

void VideoDisplayWidget::screenshot()
{
    mRecorder.saveScreenshot(boundingRect(), QString());
}

void VideoDisplayWidget::screenshotWithFileName(const QString &filename)
{
    mRecorder.saveScreenshot(boundingRect(), QString(filename));
}

void VideoDisplayWidget::recordVideo(int fps)
{
    if(!mRecorder.recording())
    {
        mToolbar->setVisible(false);
        mRecorder.startVideo(boundingRect(), fps);
    }
}

void VideoDisplayWidget::endVideoRecording(const QString &filename)
{
    if(mRecorder.recording())
        mRecorder.finishVideo(filename);
}

void VideoDisplayWidget::setZoom(double value)
{
    mScale = value;
    setZoomToFit(false);
    showInfo(tr("Zoom: %1%").arg(mScale * 100.0));
    update();
}

void VideoDisplayWidget::setZoomToFit(bool value)
{
    mScaleToFit = value;
    mToolbar->setZoomToFit(value);
    update();
}

void VideoDisplayWidget::setHorizontalFlip(bool flip)
{
    mHorizontalFlip = flip;
    mToolbar->setFlip(mHorizontalFlip, mVerticalFlip);
    update();
}

void VideoDisplayWidget::setVerticalFlip(bool flip)
{
    mVerticalFlip = flip;
    mToolbar->setFlip(mHorizontalFlip, mVerticalFlip);
    update();
}

void VideoDisplayWidget::wheelEvent(QWheelEvent *event)
{
    if (mToolbar->currentPortInfo().typeName == "Real" || mToolbar->currentPortInfo().typeName == "Integer"){
        if (event->delta() > 0)
            mIncrementScaler *= 1.148698354997035;
        else
            mIncrementScaler /= 1.148698354997035;
        showInfo(tr("New scale: %1%").arg(mIncrementScaler * 100.0));
    } else {
        if (mScaleToFit)
            return;
        double scale = mScale;
        if (event->delta() > 0)
            scale *= 1.148698354997035;
        else
            scale /= 1.148698354997035;
        setZoom(scale);
    }
    event->accept();
}

void VideoDisplayWidget::enterEvent(QEvent*)
{
    mToolbar->setVisible(true);
    update();
}

void VideoDisplayWidget::leaveEvent(QEvent*)
{
    mToolbar->setVisible(false);
    update();
}

QPoint VideoDisplayWidget::closestPoint(const QRect &bounds, QPoint point)
{
    if (point.x() < bounds.left())
        point.setX(bounds.left());
    else if (point.x() > bounds.right())
        point.setX(bounds.right());
    if (point.y() < bounds.top())
        point.setY(bounds.top());
    else if (point.y() > bounds.bottom())
        point.setY(bounds.bottom());
    return point;
}
