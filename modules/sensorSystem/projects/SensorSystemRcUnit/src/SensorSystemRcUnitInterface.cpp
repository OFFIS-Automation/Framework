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

#include "SensorSystemRcUnitInterface.h"
#include "SensorSystemRcUnit.h"
#include <QtPlugin>

SensorSystemRcUnitInterface::SensorSystemRcUnitInterface()
{
}

QString SensorSystemRcUnitInterface::description() const
{
    QString desc = "Allows for the manupulation of traced signals from the automation.";
    desc += "<br />Additionally allows export of traces.";
    return desc;
}

UserRcUnit *SensorSystemRcUnitInterface::instance(RcBase &rc, const QString &, const QString &)
{
    rc.addMethod("getAllSensors", "Retuns a list of sensor names", "Returns a list of active and inactive sensors.");
    rc.addMethod("getActiveSensors", "Retuns a list of sensor names", "Returns a list of all active sensors.");
    rc.addMethod("getTracedSensors", "Returns a list of sensor names", "Returns a list of all sensor names that are currently traced");
    rc.addMethod("addToTrace", "Starts tracing of a sensor", "Starts tracing of the sensor idnetified by the given name");
    rc.addMethod("removeFromTrace", "Stops tracing of a sensor", "Stops tracing of the sensor idnetified by the given name");
    rc.addMethod("addMarker", "Adds a marker to the trace", "Add a marker to the trace file. A marker can be used to exoprt only pieces of a trace. The marker named is added with a unique id. The real name of the marker is returned!");
    rc.addMethod("setSeperator", "Sets the seperator for the export", "The given seperator will be used between data points in the exported text file. The deafult is a comma.");
    rc.addMethod("setStartAtZero", "Sets this attribute for export", "If startAtZero is set to true, every export will start at the time 0. If it is false (the default), it will be the seconds since the system start.");
    rc.addMethod("setExportRange", "Sets the range for the export", "Sets the range using markers. You can give a marker name for start and/or end, or a empty string. An empty string as startMArker means start at beginning of trace; An empty string as aned marker means end of trace.");
    rc.addMethod("setExportSensors", "Sets the sensor for the next export", "Only the given sensors will be included in the next export.");
    rc.addMethod("setExportSensors2", "Sets the sensor for the next export", "Only the given sensors will be included in the next export. A new line will be written only for the sensitiveSensor");
    rc.addMethod("exportTrace", "Exports a trace to a file", "Exports a trace to the given file. Use setSeperator, setStartAtZero, setExportRange and setExportSensors to define the trace.");
    return new SensorSystemRcUnit();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(SensorSystemRcUnitInterface, SensorSystemRcUnitInterface)
#endif
