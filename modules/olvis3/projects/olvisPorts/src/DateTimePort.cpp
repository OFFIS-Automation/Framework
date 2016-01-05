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
// along with this program.  If not, see <http:#www.gnu.org/licenses/>.


#include <ports/DateTimePort.h>

port::DateTime::DateTime() : OverlayPort(QVariant::DateTime, "DateTime")
{
    setIcon(QImage(":/defaultPorts/images/time.png"));
}

QDateTime in::DateTime::getValue()
{
    return getRawValue().toDateTime();
}

void in::DateTime::setDefault(const QDateTime& value)
{
    setDefaultValueRaw(value);
}

QString port::DateTime::toString(const QVariant& var) const
{
    return var.toDateTime().toString(format());
}

QVariant port::DateTime::fromString(const QString& str) const
{
    QDateTime dt = QDateTime::fromString(str, format());
    if(!dt.isValid())
        dt = QDateTime::fromString(str);
    return dt;
}

void out::DateTime::send(const QDateTime& value)
{
    sendRaw(value);
}
