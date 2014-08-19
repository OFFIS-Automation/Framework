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

#ifndef DIRECTORYPORT_H
#define DIRECTORYPORT_H

#include "DefaultPortsGlobal.h"
#include <QDir>

namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Directory : public Port
    {
    public:
        static QVariant variant(const QDir& dir);
        static QDir fromVariant(const QVariant& var);
        void setDefault(const QDir& dir);
    protected:
        Directory();
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const { return var; }
        QVariant toSimpleType(const QVariant& var) const { return toString(var); }
    };
}
namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Directory : public port::Directory
    {
    public:
        void send(const QDir& file);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Directory : public  port::Directory
    {
    public:
        QDir getValue();
        inline operator QDir(){ return getValue(); }
    protected:

    };
}


#endif // DIRECTORYPORT_H
