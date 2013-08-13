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

#include "Overlay.h"

#include "src/OlvisSingleton.h"
#include <core/FilterInfo.h>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include "VideoDisplayWidget.h"

Overlay::Overlay(QString name) : mWidget(0), mRate(-1.0), mActive(false), mIsOutput(true), mName(name)
{
    const OlvisInterface& model = OlvisSingleton::instance();
    connect(this, SIGNAL(listeningStarted(PortId,PortListener*)), &model, SLOT(addPortListener(PortId,PortListener*)));
    connect(this, SIGNAL(listeningStopped(PortId,PortListener*)), &model, SLOT(removePortListener(PortId,PortListener*)));
    connect(this, SIGNAL(valueChanged(PortId,QVariant)), &model, SLOT(setPortValue(PortId,QVariant)));
}

Overlay::~Overlay()
{
    if (mPortId.isValid() && mIsOutput)
        emit listeningStopped(mPortId, this);
}

void Overlay::setWidget(VideoDisplayWidget *widget)
{
    mWidget = widget;
    connect(this, SIGNAL(removeClicked(Overlay*)), widget, SLOT(removeOverlay(Overlay*)));
}

QString Overlay::name()
{
    return mName;
}

void Overlay::writeCurrentConfig(QXmlStreamWriter &writer)
{
    writer.writeAttribute("output", QString::number(mIsOutput));
    writer.writeEmptyElement("port");
    writer.writeAttribute("filter", OlvisSingleton::instance().getFilter(mPortId.filter).name);
    writer.writeAttribute("port", mPortId.port);
}

void Overlay::readConfig(QXmlStreamReader &reader)
{
    mIsOutput = reader.attributes().value("output").toString().toInt() == 1;
    while (reader.readNextStartElement()) {
        readElement(reader);
        reader.readElementText();
    }
}

void Overlay::readElement(QXmlStreamReader &reader)
{
    if (reader.name() == "port") {
        PortId portId;
        portId.filter = OlvisSingleton::instance().getFilter(reader.attributes().value("filter").toString()).id;
        portId.port = reader.attributes().value("port").toString();
        setPortId(portId, mIsOutput);
    }
}

void Overlay::setActive(bool active)
{
    mActive = active;
}

QList<QPoint> Overlay::snapPoints()
{
    return QList<QPoint>();
}

double Overlay::rate()
{
    return mRate;
}

void Overlay::setPortId(const PortId &portId, bool output)
{
    mIsOutput = output;
    if (mIsOutput) {
        if (mPortId.isValid())
            emit listeningStopped(mPortId, this);
        if (portId.isValid()) {
            emit listeningStarted(portId, this);
            mLastValue = OlvisSingleton::instance().getPortValue(portId);
        }
    } else {
        if (portId.isValid())
            setValue(OlvisSingleton::instance().getPortValue(portId));
    }
    mPortId = portId;
}

bool Overlay::isOutput() {
    return mIsOutput;
}

PortId Overlay::portId()
{
    return mPortId;
}

void Overlay::setValue(const QVariant &value)
{
    if(!mMutex.tryLock(5))
        return;
    //QMutexLocker lock(&mMutex);
    mLastValue = value;
    mMutex.unlock();
    double val = mTimer.restart();
    if (mRate == -1.0)
        mRate = val;
    else
        mRate = mRate * 0.8 + val * 0.2;
    //qDebug() << val << mRate;
    if (mWidget)
        mWidget->dataChanged(this);
}

QRect Overlay::boundingRect() {
    return QRect();
}

