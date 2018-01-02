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

#ifndef PORTDATA_H
#define PORTDATA_H

#include "InputPortPrivate.h"
#include "OutputPortPrivate.h"
#include <core/PortInfo.h>
class PortData
{
public:
    PortData() : in(0), out(0) {}
    PortInfo info;
    InputPort* in;
    OutputPort* out;
    // lists for contstraints
    QList<QVariant> choiceValues;
    QStringList choiceNames;

};

#endif // PORTDATA_H
