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

#include "ConfigReader.h"
#include <core/OlvisInterface.h>
#include <core/FilterInfo.h>
#include <core/ProcessorInfo.h>
#include <QThread>

ConfigReader::ConfigReader(OlvisInterface& control, const QString& data) : QXmlStreamReader(data), mInterface(control)
{
}

ConfigReader::ConfigReader(OlvisInterface& control, QIODevice* device) : QXmlStreamReader(device), mInterface(control)
{
}

void ConfigReader::createConfig()
{
    bool inProcessingMode = false;
    while(!atEnd())
    {
        TokenType token = readNext();
        // first, check if processing is possible
        if(token == StartElement && name() == "olvisConfig")
        {
            inProcessingMode = true;
            continue;
        }
        if(!inProcessingMode)
            continue;

        switch(token)
        {
        case StartElement:
            if(name() == "makroFilter")
                createMakroFilter();
            else if(name() == "processor")
                createProcessor();
            else if(name() == "buffer")
                createBuffer();
            else if(name() == "join")
                createJoin();
            else if(name() == "filter")
                createFilter();
            else if(name() == "port")
                setPort();
            else if(name() == "makroInput")
                addMakroInput();
            else if(name() == "makroOutput")
                addMakroOutput();
            else if(name() == "processorOutput")
                addProcessorOutput();
            else if(name() == "source")
                setSource();
            else if(name() == "connection")
                connectProcessors();
            else if(name() == "inputConnection")
                connectProcessorInput();
            mCurrentData = "";
            break;
        case Characters:
            mCurrentData += text();
            break;
        case EndElement:
            if(name() == "processor")
                mCurrentProcessor = 0;
            else if(name() == "filter")
                mCurrentFilter = 0;
            else if(name() == "port")
                mCurrentPort = "";
            else if(name() == "value")
                setValue();
            else if(name() == "olvisConfig")
                inProcessingMode = false;
            break;
        default:
            break;
        }
    }
}

void ConfigReader::createMakroFilter()
{
    QString name = attributes().value("name").toString();
    mCurrentProcessor = mInterface.createMakroFilter(name);
}

void ConfigReader::createProcessor()
{
    QString name = attributes().value("name").toString();
    QString prioString = attributes().value("priority").toString();
    QThread::Priority prio = (QThread::Priority)prioString.toInt();
    bool pausedStartup = attributes().value("pausedStartup").toString().toInt() != 0;
    bool ignoreTrigger = attributes().value("ignoreTrigger").toString().toInt() != 0;
    mCurrentProcessor = mInterface.createProcessor(name);
    mInterface.setProcessorPriority(mCurrentProcessor, prio);
    mInterface.setProcessorStartupBehavior(mCurrentProcessor, pausedStartup);
    mInterface.setProcessorTriggerBehavior(mCurrentProcessor, ignoreTrigger);
    mProcessingElementIds[name] =  mCurrentProcessor;
}

void ConfigReader::createBuffer()
{
    QString name = attributes().value("name").toString();
    QString sizeString = attributes().value("size").toString();
    mProcessingElementIds[name] = mInterface.createDataBuffer(sizeString.toInt(), name);
}

void ConfigReader::createJoin()
{
    QString name = attributes().value("name").toString();
    QString modeString = attributes().value("mode").toString();
    QString masterString = attributes().value("master").toString();
    int id = mInterface.createJoin(name);
    ProcessorInfo info = mInterface.getProcessor(masterString);
    mInterface.setJoinMode(id, modeString.toInt(), info.id);
    mProcessingElementIds[name] = id;
}

void ConfigReader::createFilter()
{
    QString name = attributes().value("name").toString();
    QString type = attributes().value("type").toString();
    mCurrentFilter = mInterface.addFilter(mCurrentProcessor, type, name);
}

void ConfigReader::setPort()
{
    mCurrentPort = attributes().value("name").toString();
}

void ConfigReader::addProcessorOutput()
{
    QString portName = attributes().value("port").toString();
    QString name = attributes().value("name").toString();
    mInterface.createProcessorOutput(mCurrentFilter, portName, name);
}

void ConfigReader::addMakroOutput()
{
    QString portName = attributes().value("port").toString();
    QString filterName = attributes().value("filter").toString();
    int id = mInterface.getFilter(filterName).id;
    QString name = attributes().value("name").toString();
    mInterface.createProcessorOutput(id, portName, name);
}

void ConfigReader::addMakroInput()
{
    QString portName = attributes().value("port").toString();
    QString filterName = attributes().value("filter").toString();
    int id = mInterface.getFilter(filterName).id;
    QString name = attributes().value("name").toString();
    mInterface.connectProcessorInput(name, id, portName);
    mInterface.renameInput(PortId(id, portName), name);
}

void ConfigReader::setSource()
{
    QString filterName = attributes().value("filter").toString();
    QString port = attributes().value("port").toString();
    int filterId = mInterface.getFilter(filterName).id;
    mInterface.connectFilter(filterId, port, mCurrentFilter, mCurrentPort);
}

void ConfigReader::setValue()
{
    QVariant data = mInterface.portValueFromString(mCurrentFilter, mCurrentPort, mCurrentData);
    mInterface.setPortValue(mCurrentFilter, mCurrentPort, data);
}

void ConfigReader::connectProcessors()
{
    QString sourceName = attributes().value("source").toString();
    QString targetName = attributes().value("target").toString();
    int source = mProcessingElementIds.value(sourceName, 0);
    int target = mProcessingElementIds.value(targetName, 0);
    mInterface.connectProcessor(source, target);
}

void ConfigReader::connectProcessorInput()
{
    QString sourceName = attributes().value("source").toString();
    QString filterName = attributes().value("targetFilter").toString();
    QString portName = attributes().value("targetPort").toString();
    int filterId = mInterface.getFilter(filterName).id;
    mInterface.connectProcessorInput(sourceName, filterId, portName);
}
