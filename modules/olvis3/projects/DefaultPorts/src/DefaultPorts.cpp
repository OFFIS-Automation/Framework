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

#include "DefaultPorts.h"

#include <QMetaType>
#include <QtPlugin>
#include <core/PortInfo.h>

#include "BoolEdit.h"
#include "FileEdit.h"
#include "DirectoryEdit.h"
#include "StringEdit.h"
#include "ChoiceEdit.h"
#include "DoubleEdit.h"
#include "NumberEdit.h"
#include "RectEdit.h"
#include "PointEdit.h"

DefaultPorts::DefaultPorts()
{
}

PortEditInterface* DefaultPorts::portEditFor(const PortInfo& info)
{
    if(info.isArray)
        return 0;
    if(info.constraints.contains("choices"))
        return new ChoiceEdit();
    else if(info.typeName == "Boolean")
        return new BoolEdit();
    else if(info.typeName == "File")
        return new FileEdit();
    else if(info.typeName == "Directory")
        return new DirectoryEdit();
    else if(info.typeName == "String")
        return new StringEdit();
    else if(info.typeName == "Integer")
        return new NumberEdit();
    else if(info.typeName == "Real")
        return new DoubleEdit();
    else if(info.typeName == "Rect")
        return new RectEdit();
    else if(info.typeName == "Point")
        return new PointEdit();
    else
        return 0;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DefaultPorts, DefaultPorts);
#endif
