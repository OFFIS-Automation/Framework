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

#ifndef STRINGPORT_H
#define STRINGPORT_H

#include "DefaultPortsGlobal.h"
#include <QStringList>

namespace port
{
    class DEFAULTPORTSSHARED_EXPORT String : public Port
    {
    public:
        void setDefault(const QString& value);

    protected:
        String();
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const { return var; }
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT String : public port::String
    {
    public:
        void send(const QString& value);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT String : public port::String
    {
    public:
        QString getValue();
        void addChoice(const QString& str);
        void addChoice(const QString& str, const QString& name);
        inline operator QString() { return getValue(); }

    };
}

#endif // INTEGERPORT_H
