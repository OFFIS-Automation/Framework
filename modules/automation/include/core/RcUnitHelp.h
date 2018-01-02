// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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
#include "../rc/types/RcUnitTypes.h"

struct RcUnitHelp : TelecontrolConfig
{
    QString desc;
    QString server;

    struct Parameter
    {
        enum Type
        {
            Single,
            List,
            Struct
        } type;
        QByteArray typeName, realTypeName;
        QString name;
        QString sig;
        int typeId;
        int min, max; // for lists/repeatables
    };

    struct Method
    {
        bool hiddenForScratch;
        QString name;
        QString shortDesc;
        QString longDesc;
        QString sig;
        bool hasReturn;
        Parameter returnParameter;
        QList<Parameter> parameters;
        QStringList parameterNames;
    };

    struct Struct
    {
        QString name;
        QList<QString> members;
    };

    QList<Struct> structs;
    QList<Method> methods;
    QVariantMap constants;
    UserRcUnitType type;
    RcFlagDefinitions flags;
    bool hwConnected;
    QVariantMap userInfo;
};


inline QDataStream& operator>>(QDataStream& stream, RcUnitHelp::Parameter& helpParamater)
{
    stream >> helpParamater.typeName;
    stream >> helpParamater.realTypeName;
    stream >> helpParamater.name;
    stream >> helpParamater.sig;
    stream >> helpParamater.min;
    stream >> helpParamater.max;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const RcUnitHelp::Parameter& helpParamater)
{
    stream << helpParamater.typeName;
    stream << helpParamater.realTypeName;
    stream << helpParamater.name;
    stream << helpParamater.sig;
    stream << helpParamater.min;
    stream << helpParamater.max;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, RcUnitHelp::Method& helpMethod)
{
    stream >> helpMethod.hiddenForScratch;
    stream >> helpMethod.name;
    stream >> helpMethod.shortDesc;
    stream >> helpMethod.longDesc;
    stream >> helpMethod.sig;
    stream >> helpMethod.hasReturn;
    stream >> helpMethod.returnParameter;
    stream >> helpMethod.parameters;
    stream >> helpMethod.parameterNames;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const RcUnitHelp::Method& helpMethod)
{
    stream << helpMethod.hiddenForScratch;
    stream << helpMethod.name;
    stream << helpMethod.shortDesc;
    stream << helpMethod.longDesc;
    stream << helpMethod.sig;
    stream << helpMethod.hasReturn;
    stream << helpMethod.returnParameter;
    stream << helpMethod.parameters;
    stream << helpMethod.parameterNames;
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
    TelecontrolConfig& tc = help;
    stream >> tc;
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
