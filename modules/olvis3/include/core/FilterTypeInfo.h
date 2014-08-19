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

#ifndef FILTERTYPEINFO_H
#define FILTERTYPEINFO_H

#include <QString>
#include <QStringList>

#include <core/PortInfo.h>

struct FilterTypeInfo
{
    enum Type
    {
        NoSpecialType       = 0,
        ColorImageType      = 0x01,
        GrayscaleImageType  = 0x02,
        ToolType            = 0x04,
        DefaultType = ToolType,
    };
    Q_DECLARE_FLAGS(Types, Type);

    QString name;
    QString plugin;
    QString desc;
    QStringList group;
    Types types;
    QList<PortInfo> inputs;
    QList<PortInfo> outputs;
    QString uid;

    bool isValid() { return !name.isEmpty(); }
};

#endif // FILTERTYPEINFO_H
