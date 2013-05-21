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

#include <ports/FilePort.h>
#include <QStringList>

#if QT_VERSION < 0x050000
Q_DECLARE_METATYPE (QFileInfo)
#endif

QVariant port::File::variant(const QFileInfo &val)
{
    return QVariant::fromValue<QFileInfo>(val);
}

QFileInfo port::File::fromVariant(const QVariant &val)
{
    return val.value<QFileInfo>();
}



port::File::File() : Port(qMetaTypeId<QFileInfo>(), "File")
{
    setIcon(QImage(":/defaultPorts/images/file.png"));
}

void port::File::setDefault(const QFileInfo& dir)
{
    setDefaultValueRaw(variant(dir));
}
QString port::File::toString(const QVariant& var) const
{
    return fromVariant(var).absoluteFilePath();
}

QVariant port::File::fromString(const QString& str) const
{
    return variant(QFileInfo(str));
}

void in::File::setFilter(const QString &filterString)
{
    QStringList filter = filterString.split(" ");
    setConstraint("filter", filter);
}

QFileInfo in::File::getValue()
{
    return fromVariant(getRawValue());
}

void out::File::send(const QFileInfo &file)
{
    sendRaw(variant(file));
}

in::WriteFile::WriteFile()
{
    setConstraint("readMode", false);
}

in::ReadFile::ReadFile()
{
    setConstraint("readMode", true);
}
