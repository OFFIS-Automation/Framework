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

#include "OlvisRcUnitInterface.h"

#include <QtPlugin>
#include <OlvisCoreInterface.h>
#include "OlvisRcUnit.h"

OlvisRcUnitInterface::OlvisRcUnitInterface()
{
}

QString OlvisRcUnitInterface::description() const
{
    QString description;
    description += tr("An automation plugin to control the vision part of the framework.");
    return description;
}

UserRcUnit *OlvisRcUnitInterface::instance(RcBase &rc, const QString &configFile, const QString& /* name */)
{
    Q_UNUSED(configFile);
    if(!OlvisCoreInterface::hasInstance()){
        return 0;
    }

    rc.addMethod("getPortValue", tr("Returns a port value"), tr("Returns the last value of the specified port."));
    rc.addMethod("getNextPortValue", tr("Returns the next fresh port value"), tr("Waits until the port fires and returns the fired port value."));
    rc.addMethod("getFuturePortValue", tr("Skips several values before returning one"), tr("Waits until the port has fired <em>skipValues</em> time and returns the next fresh port value.<br /> skipValues = 0 is identical to getNextPortValue."));
    rc.addMethod("setPortValue", tr("Sets a port value"), tr("Sets a port value, only possible for input ports. For connected ports, the value is not set. Instead, the default value is set."));
    rc.addMethod("pauseProcessor", tr("Pauses a processor"), tr("Pauses a processor, can be restarted with resumeProcessor."));
    rc.addMethod("resumeProcessor", tr("Resumes a processor"), tr("Restarts a previously paused processor. Does nothing if the processor is running."));

    rc.addMethod("isRunning", tr("Returns the running status"), tr("Returns wether the Vision system is ruunning or stopped/finished"));
    rc.addMethod("start", tr("Starts vision if not running"), tr("Starts vision chain if not running. Start all processors if vision was not running."));
    rc.addMethod("stop", tr("Stops vision if running"), tr("Stops the vision chain if running. Stops all processors."));

    rc.addMethod("getProcessorOutput", tr("Returns a map of all processor outputs"), tr("Returns a map with string keys. The keys correspond to the processor output names.<br />All values are from the same processor iteration."));
    rc.addMethod("getNextProcessorOutput", tr("Waits until a processor is finished and returns a map of all processor outputs"), tr("See getProcessorOutput for more details."));
    rc.addMethod("getFutureProcessorOutput", tr("Waits several processor iterations and returns a map of all processor outputs"), tr("See getProcessorOutput for more details."));
    return new OlvisRcUnit();
}
