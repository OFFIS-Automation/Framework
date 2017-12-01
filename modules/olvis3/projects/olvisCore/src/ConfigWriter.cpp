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

#include "ConfigWriter.h"
#include "OlvisCoreInterface.h"
#include <core/ProcessorInfo.h>
#include <core/FilterInfo.h>

#include <QXmlStreamWriter>

ConfigWriter::ConfigWriter(const OlvisCoreInterface &model)
    : mInterface(model)
{
}

QString ConfigWriter::toXml(const QString& targetFile)
{
    QString buffer;
    {
        QXmlStreamWriter xml(&buffer);
        xml.setAutoFormatting(true);
        toXml(&xml, targetFile);
    }
    return buffer;

}
void ConfigWriter::toXml(QXmlStreamWriter *xml, const QString &targetFile)
{
    targetPath = QFileInfo(targetFile).dir();
    comparePath = targetPath;
    comparePath.cdUp();
    comparePath.cdUp();
    stream = xml;
    stream->writeStartElement("olvisConfig");
    addMakroFilter();
    addProcessors();
    addJoins();
    addBuffers();
    addConnections();
    stream->writeEndElement();
}

void ConfigWriter::addMakroFilter()
{
    foreach(const FilterGroupInfo& makro, mInterface.getMakroFilters())
    {
        stream->writeStartElement("makroFilter");
        stream->writeAttribute("name", makro.name);
        foreach(FilterInfo filter, mInterface.getFilters(makro.id))
        {
            stream->writeStartElement("filter");
            stream->writeAttribute("name", filter.name);
            stream->writeAttribute("type", filter.typeInfo.uid);
            addInputs(filter);
            stream->writeEndElement();
        }
        foreach(const FilterGroupPort& portInfo, makro.inputs)
        {
            stream->writeStartElement("makroInput");
            stream->writeAttribute("name", portInfo.name);
            stream->writeAttribute("filter", mInterface.getFilter(portInfo.filterId).name);
            stream->writeAttribute("port", portInfo.port.name);
            stream->writeEndElement();
        }
        foreach(const FilterGroupPort& portInfo, makro.outputs)
        {
            stream->writeStartElement("makroOutput");
            stream->writeAttribute("name", portInfo.name);
            stream->writeAttribute("filter", mInterface.getFilter(portInfo.filterId).name);
            stream->writeAttribute("port", portInfo.port.name);
            stream->writeEndElement();
        }

        stream->writeEndElement();
    }
}


void ConfigWriter::addProcessors()
{
    foreach(ProcessorInfo processor, mInterface.getProcessors())
    {
        stream->writeStartElement("processor");
        stream->writeAttribute("name", processor.name);
        stream->writeAttribute("priority", QString::number(processor.priority));
        stream->writeAttribute("pausedStartup", processor.pausedStartup ? "1" : "0");
        stream->writeAttribute("stopOnNoOutput", processor.stopOnNoOutput ? "1" : "0");
        stream->writeAttribute("ignoreTrigger", processor.ignoreTrigger ? "1" : "0");
        foreach(FilterInfo filter, mInterface.getFilters(processor.id))
        {
            stream->writeStartElement("filter");
            stream->writeAttribute("name", filter.name);
            stream->writeAttribute("type", filter.typeInfo.uid);
            addInputs(filter);
            stream->writeEndElement();
        }
        stream->writeEndElement();
    }
}

void ConfigWriter::addInputs(const FilterInfo &filter)
{
    QList<FilterGroupPort> outputs = mInterface.processorOutputs(filter.processorId);
    foreach(FilterGroupPort pId, outputs)
    {
        if(pId.filterId != filter.id)
            continue;
        stream->writeStartElement("processorOutput");
        stream->writeAttribute("port", pId.port.name);
        stream->writeAttribute("name", pId.name);
        stream->writeEndElement();
    }

    foreach(PortInfo port, filter.typeInfo.inputs)
    {
        PortId source = mInterface.getSource(PortId(filter.id, port.name));
        QVariant value = mInterface.getPortValue(PortId(filter.id, port.name));
        if(!source.isValid() && !value.isValid())
            continue;
        stream->writeStartElement("port");
        stream->writeAttribute("name", port.name);
        if(value.isValid())
        {
            QString strval = mInterface.portValueString(PortId(filter.id, port.name), value);
            stream->writeStartElement("value");
            stream->writeCharacters(strval);
            stream->writeEndElement();
            if(strval.contains(comparePath.absolutePath()))
            {
                stream->writeStartElement("relativePathValue");
                stream->writeAttribute("type", QMetaType::typeName(value.userType()));
                stream->writeCharacters(targetPath.relativeFilePath(strval));
                stream->writeEndElement();
            }
        }
        if(source.isValid())
        {
            FilterInfo info = mInterface.getFilter(source.filter);
            stream->writeStartElement("source");
            stream->writeAttribute("filter", info.name);
            stream->writeAttribute("port", source.port);
            stream->writeEndElement();
        }
        stream->writeEndElement();
    }
}

void ConfigWriter::addJoins()
{
    foreach(JoinInfo join, mInterface.getJoins())
    {
        stream->writeStartElement("join");
        stream->writeAttribute("name", join.name);
        stream->writeAttribute("mode", QString::number(join.joinMode));
        ProcessorInfo pInfo = mInterface.getProcessor(join.master);
        if(pInfo.isValid())
            stream->writeAttribute("master", pInfo.name);
        stream->writeEndElement();
    }
}

void ConfigWriter::addBuffers()
{
    foreach(BufferInfo buffer, mInterface.getDataBuffers())
    {
        stream->writeStartElement("buffer");
        stream->writeAttribute("name", buffer.name);
        stream->writeAttribute("size", QString::number(buffer.size));
        stream->writeEndElement();
    }
}

void ConfigWriter::addConnections()
{
    foreach(ProcessingElementConnection conn, mInterface.processingElementConnections())
    {
        QString sourceName = "", targetName = "";
        if(conn.sourceType == ProcessorElementType)
            sourceName = mInterface.getProcessor(conn.sourceId).name;
        else if(conn.sourceType == JoinElementType)
            sourceName = mInterface.getJoin(conn.sourceId).name;
        else if(conn.sourceType == BufferElementType)
            sourceName = mInterface.getDataBuffer(conn.sourceId).name;

        if(conn.targetType == ProcessorElementType)
            targetName = mInterface.getProcessor(conn.targetId).name;
        else if(conn.targetType == JoinElementType)
            targetName = mInterface.getJoin(conn.targetId).name;
        else if(conn.targetType == BufferElementType)
            targetName = mInterface.getDataBuffer(conn.targetId).name;
        stream->writeStartElement("connection");
        stream->writeAttribute("source", sourceName);
        stream->writeAttribute("target", targetName);
        stream->writeEndElement();
        QList<QString> inputs = mInterface.processorInputs(conn.targetId);
        foreach(QString input, inputs)
        {
            QList<PortId> targets = mInterface.processorInputTargets(conn.targetId, input);
            foreach(PortId target, targets)
            {
                stream->writeStartElement("inputConnection");
                stream->writeAttribute("source", input);
                FilterInfo info = mInterface.getFilter(target.filter);
                stream->writeAttribute("targetFilter", info.name);
                stream->writeAttribute("targetPort", target.port);
                stream->writeEndElement();
            }
        }
    }
}
