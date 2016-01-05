// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef LINEPORT_H
#define LINEPORT_H

#include "DefaultPortsGlobal.h"
#include <QLineF>

namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Line : public Port
    {
    public:
        void setDefault(const QLineF& Point);
    protected:
        Line();
        QVariant toSimpleType(const QVariant &var) const;
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const;
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Line : public port::Line
    {
    public:
        void send(const QLineF& point);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Line : public port::Line
    {
    public:
        QLineF getValue();
        inline operator QLineF() { return getValue(); }
    };
}

#endif // LINEPORT_H
