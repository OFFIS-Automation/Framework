// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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


#ifndef NO_SENSOR_SYSTEM

#include "SensorSystemOverlay.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>

#include <SensorTraceExport.h>

SensorSystemOverlay::SensorSystemOverlay(const QString &name) :
    GraphOverlay(name)
{
    mShowHistory = true;
}

SensorSystemOverlay::~SensorSystemOverlay()
{
    if(!mSensorId.isEmpty())
    {
        DataConsumer& receiver = SensorSystemInterface::consumer(mSensorId);
        receiver.disconnect(this);
    }
}

void SensorSystemOverlay::setPortId(const PortId &id, bool output)
{
    Q_UNUSED(output);
    mIsOutput = true;
    setSensorId(id.port);
}

void SensorSystemOverlay::setSensorId(const QString &sensorId)
{
    mSensorId = sensorId;
    DataConsumer& receiver = SensorSystemInterface::consumer(sensorId);
    connect(&receiver, SIGNAL(updated(QVariant,double,int)), SLOT(onNewValue(QVariant)), Qt::DirectConnection);
}

void SensorSystemOverlay::writeCurrentConfig(QXmlStreamWriter &writer)
{
    GraphOverlay::writeCurrentConfig(writer);
    writer.writeTextElement("sensorId", mSensorId);
}

void SensorSystemOverlay::readElement(QXmlStreamReader &reader)
{
    if(reader.name() == "sensorId")
        setSensorId(reader.readElementText().trimmed());
    else
        GraphOverlay::readElement(reader);
}



void SensorSystemOverlay::onNewValue(QVariant value)
{
    setValue(SensorTraceExport::convertToDouble(value));

}

#endif
