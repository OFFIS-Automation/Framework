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
// along with this program.  If not, see <http:#www.gnu.org/licenses/>.

#ifndef DATETIMEPORT_H
#define DATETIMEPORT_H

#include "DefaultPortsGlobal.h"
#include <QDateTime>

namespace port
{
    class DEFAULTPORTSSHARED_EXPORT DateTime : public OverlayPort
    {
    protected:
        DateTime();
        static QString format() { return "yyyy/MM/dd hh:mm:ss:zzz"; }
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const { return var; }
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT DateTime : public port::DateTime
    {
    public:
        void send(const QDateTime& value);
    };
};

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT DateTime : public port::DateTime
    {
    public:
        QDateTime getValue();
        inline operator QDateTime() { return getValue(); }
        void setDefault(const QDateTime& value);
    };
}

#endif // DATETIMEPORT_H
