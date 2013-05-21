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

#include "CustomLolec.h"
#include <QDebug>

CustomLolec::CustomLolec()
{
}

void CustomLolec::acquireHardware()
{
    //@TODO: implement
    qDebug() << "CustomLolec" << "connect called";
    bool success = true;
    emit connectStatusChanged(success);
}

void CustomLolec::releaseHardware()
{
    //@TODO: implement
    qDebug() << "CustomLolec" << "disconnect called";
    emit connectStatusChanged(false);
}

void CustomLolec::moveGamepad(double xAxis, double yAxis)
{
    //if this is connected to a gamepad method, the values for xAxis and yAxis are between -1.0 and 1.0
    //@TODO: implement
}
