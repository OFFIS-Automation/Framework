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

#ifndef PROCESSORINFO_H
#define PROCESSORINFO_H

#include <QString>
#include <QThread>
#include <QTime>
#include <QDebug>
#include "PortInfo.h"

enum ProcessingElementType
{
    InvalidElementType,
    ProcessorElementType,
    BufferElementType,
    JoinElementType
};

struct FilterGroupPort
{
    int parentId;
    QString name;
    int filterId;
    PortInfo port;
    bool isValid(){ return !name.isEmpty() && port.isValid(); }
};

struct FilterGroupInfo
{
    int id;
    QString name;
    QList<FilterGroupPort> outputs;
    QList<FilterGroupPort> inputs;
    bool isValid() { return id > 0; }
    FilterGroupInfo() : id(0) {}
};

struct ProcessorInfo : FilterGroupInfo
{
    QThread::Priority priority;
    bool pausedStartup;
    bool ignoreTrigger;
    ProcessorInfo() : priority(QThread::NormalPriority), pausedStartup(false), ignoreTrigger(false) {}
};

struct BufferInfo
{
    int id;
    int size;
    QString name;
    BufferInfo() : id(0) {}
    bool isValid() const { return id > 0; }
};

struct JoinInfo
{
    enum JoinMode
    {
        JoinMasterMode, //! one of the connected inputs acts as master, the join fires if the master fires
        JoinAnyMode, //! the join fires if any of the inputs sends new data
        JoinAllMode, //! the join fires if all of the inputy have send new data since the last fireing
        JoinDefaultMode = JoinAllMode,
        JoinFirstMode = JoinMasterMode,
        JoinLastMode = JoinAllMode
    };

    int id;
    QString name;
    int joinMode;
    int master;
    JoinInfo() : id(0) {}
    bool isValid() const { return id > 0; }
};

struct ProcessingElementConnection
{
    ProcessingElementType sourceType;
    int sourceId;
    ProcessingElementType targetType;
    int targetId;
};

#endif // PROCESSORINFO_H
