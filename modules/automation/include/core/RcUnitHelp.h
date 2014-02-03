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



#endif // RCUNITHELP_H
