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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef FILEPORT_H
#define FILEPORT_H

#include "DefaultPortsGlobal.h"
#include <QFileInfo>

namespace port
{
    class DEFAULTPORTSSHARED_EXPORT File: public Port
    {
    public:
        static QVariant variant(const QFileInfo& info);
        static QFileInfo fromVariant(const QVariant& variant);
        void setDefault(const QFileInfo& dir);
    protected:
        File();
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const { return var; }
        QVariant toSimpleType(const QVariant& var) const { return toString(var); }
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT File: public  port::File
    {
    public:
        void send(const QFileInfo& file);

    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT File : public port::File
    {
    public:
        void setMode(QIODevice::OpenMode mode);
        QFileInfo getValue();
        inline operator QFileInfo() { return getValue(); }
        void setFilter(const QString& filter);
    protected:
        File() {};
    };

    class DEFAULTPORTSSHARED_EXPORT ReadFile :  public File
    {
    public:
        ReadFile();
    };

    class DEFAULTPORTSSHARED_EXPORT WriteFile :  public File
    {
       public:
        WriteFile();
    };
}

#endif // FILEPORT_H
