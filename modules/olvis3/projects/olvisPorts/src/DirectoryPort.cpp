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

#include <ports/DirectoryPort.h>


Q_DECLARE_METATYPE(QDir);

QVariant port::Directory::variant(const QDir &val)
{
    return QVariant::fromValue<QDir>(val);
}

QDir port::Directory::fromVariant(const QVariant& variant)
{
    return variant.value<QDir>();
}


port::Directory::Directory() : Port(qMetaTypeId<QDir>(), "Directory")
{
    setIcon(QImage(":/defaultPorts/images/folder.png"));
}

void port::Directory::setDefault(const QDir& dir)
{
    setDefaultValueRaw(variant(dir));
}

QString port::Directory::toString(const QVariant& var) const
{
    return fromVariant(var).absolutePath();
}

QVariant port::Directory::fromString(const QString& str) const
{
    return variant(QDir(str));
}

QDir in::Directory::getValue()
{
    return fromVariant(getRawValue());
}

void out::Directory::send(const QDir &Directory)
{
    sendRaw(QVariant::fromValue<QDir>(Directory));
}

