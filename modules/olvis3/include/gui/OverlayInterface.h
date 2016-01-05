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
// along with this program.  If not, see <http:#www.gnu.org/licenses/>.

#ifndef OVERLAYINTERFACE_H
#define OVERLAYINTERFACE_H

#include <QString>
#include <QObject>
#include <QPainter>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <core/PortId.h>
#include <QWidget>

class OverlayParent
{
public:
    virtual QSize imageSize() = 0;
    virtual QWidget* overlayParentWidget() = 0;
};

class QMouseEvent;

class OverlayInterface : public QObject
{
    Q_OBJECT
public:
    virtual QString name() = 0;
    virtual void setParent(OverlayParent* parent) = 0;
    virtual PortId portId() = 0;
    virtual void setPortId(const PortId& id, bool output) = 0;
    virtual void setInitialPos(const QPoint& p) { Q_UNUSED(p); }
    virtual QRect boundingRect() = 0;
    virtual void setValue(const QVariant& var) = 0;
    virtual bool contains(const QPoint& p) = 0;
    virtual QList<QPoint> snapPoints() { return QList<QPoint>(); }
    virtual bool isOutput() { return true; }
    virtual void writeCurrentConfig(QXmlStreamWriter& writer){Q_UNUSED(writer);}
    virtual void readConfig(QXmlStreamReader& reader){Q_UNUSED(reader);}
    virtual void setActive(bool active) { Q_UNUSED(active); }
    virtual void paint(QPainter& painter, bool showControls) = 0;
    virtual void mousePressEvent(QMouseEvent* event) {Q_UNUSED(event);}
    virtual void mouseReleaseEvent(QMouseEvent*  event) {Q_UNUSED(event);}
    virtual void mouseMoveEvent(QMouseEvent*  event, QList<QPoint> snapPoints) {Q_UNUSED(event); Q_UNUSED(snapPoints);}
protected slots:
    void removeMe() { emit removeOverlay(this); }
    void updated() { emit updated(this); }
signals:
    void removeOverlay(OverlayInterface* overlay);
    void updated(OverlayInterface* overlay);
};

#endif // OVERLAYINTERFACE_H

