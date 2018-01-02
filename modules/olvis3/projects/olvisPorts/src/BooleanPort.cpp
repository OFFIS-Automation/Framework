// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include <ports/BooleanPort.h>

#include <ports/IntegerPort.h>
#include <QStringList>
#include <core/PortInfo.h>

port::Boolean::Boolean() : OverlayPort(QMetaType::Bool, "Boolean")
{
    setIcon(QImage(":/defaultPorts/images/bool.png"));
}

bool in::Boolean::getValue()
{
    return getRawValue().toBool();
}

void in::Boolean::setDefault(bool value)
{
    setDefaultValueRaw(value);
}

QString port::Boolean::toString(const QVariant& var) const
{
    return var.toString();
}

QVariant port::Boolean::fromString(const QString& str) const
{
    return QVariant(str).toBool();
}

void out::Boolean::send(bool value)
{
    sendRaw(value);
}


