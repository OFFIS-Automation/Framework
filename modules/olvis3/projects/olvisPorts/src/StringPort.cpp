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

#include <ports/StringPort.h>



port::String::String() : Port(QMetaType::QString, "String")
{
    setIcon(QImage(":/defaultPorts/images/string.png"));
}

void port::String::setDefault(const QString& value)
{
    setDefaultValueRaw(value);
}

QString port::String::toString(const QVariant& var) const
{
    return var.toString();
}

QVariant port::String::fromString(const QString& str) const
{
    return QVariant(str);
}

QString in::String::getValue()
{
    return getRawValue().toString();
}

void out::String::send(const QString& value)
{
    sendRaw(value);
}

void in::String::addChoice(const QString &str)
{
    addChoice(str, str);
}

void in::String::addChoice(const QString &str, const QString &name)
{
    addChoiceRaw(str, name);
}
