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

#ifndef RCUNITHELP_H
#define RCUNITHELP_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include "TelecontrolConfig.h"

struct RcUnitHelp : TelecontrolConfig
{
    QString desc;
    QString server;
    struct Method
    {
        QString name;
        QString sig;
        QString shortDesc;
        QString longDesc;
    };

    struct Struct
    {
        QString name;
        QList<QString> members;
    };
    QList<Struct> structs;
    QList<Method> methods;
    QVariantMap constants;
};

inline QDataStream& operator>>(QDataStream& stream, RcUnitHelp::Method& helpMethod)
{
    stream >> helpMethod.name;
    stream >> helpMethod.sig;
    stream >> helpMethod.shortDesc;
    stream >> helpMethod.longDesc;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const RcUnitHelp::Method& helpMethod)
{
    stream << helpMethod.name;
    stream << helpMethod.sig;
    stream << helpMethod.shortDesc;
    stream << helpMethod.longDesc;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, RcUnitHelp::Struct& helpStruct)
{
    stream >> helpStruct.name;
    stream >> helpStruct.members;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const RcUnitHelp::Struct& helpStruct)
{
    stream << helpStruct.name;
    stream << helpStruct.members;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, RcUnitHelp& help)
{
    stream >> static_cast<TelecontrolConfig>(help);
    stream >> help.desc;
    stream >> help.server;
    stream >> help.structs;
    stream >> help.methods;
    stream >> help.constants;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const RcUnitHelp& help)
{
    stream << static_cast<TelecontrolConfig>(help);
    stream << help.desc;
    stream << help.server;
    stream << help.structs;
    stream << help.methods;
    stream << help.constants;
    return stream;
}

#endif // RCUNITHELP_H
