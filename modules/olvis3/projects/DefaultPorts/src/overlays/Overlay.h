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

#ifndef OVERLAY_H
#define OVERLAY_H

#include <core/PortListener.h>
#include <core/PortId.h>
#include <QPainter>
#include <QMutex>
#include <QList>
#include <QWidget>
//#include <Timer.h>

#include <gui/OverlayInterface.h>
#include <core/OlvisInterface.h>

class QMouseEvent;
class QXmlStreamWriter;
class QXmlStreamReader;
class VideoDisplayWidget;



class Overlay : public OverlayInterface, public PortListener
{
    Q_OBJECT
public:
    Overlay(QString name);
    virtual ~Overlay();
    void setParent(OverlayParent* parent) { mParent = parent; mWidget = parent->overlayParentWidget(); }
    void setOlvisInterface(OlvisInterface* itf);

    virtual void writeCurrentConfig(QXmlStreamWriter& writer);
    virtual void readConfig(QXmlStreamReader& reader);
    virtual void readElement(QXmlStreamReader& reader);

    virtual PortId portId();
    virtual void setPortId(const PortId& portId, bool output);
    virtual bool isOutput();

    virtual void setValue(const QVariant &value);
    virtual void setInitialPos(const QPoint& pos) = 0;

    virtual void paint(QPainter& painter, bool showControls) = 0;
    virtual QRect boundingRect();

    virtual void setActive(bool active);
    virtual void mousePressEvent(QMouseEvent* event) = 0;
    virtual void mouseReleaseEvent(QMouseEvent*  event) = 0;
    virtual void mouseMoveEvent(QMouseEvent*  event, QList<QPoint> snapPoints) = 0;

    virtual bool contains(const QPoint& pos) = 0;
    virtual QList<QPoint> snapPoints();

    virtual QString name();

signals:
    void listeningStarted(const PortId& portId, PortListener* listener);
    void listeningStopped(const PortId& portId, PortListener* listener);
    void valueChanged(const PortId& portId, const QVariant& value);

protected:
    QString mName;
    QWidget* mWidget;
    OverlayParent* mParent;
    QMutex mMutex;
    PortId mPortId;
    QVariant mLastValue;
    double mRate;

    bool mActive, mIsOutput;
    OlvisInterface* mVisionInterface;
};

#endif // OVERLAY_H
