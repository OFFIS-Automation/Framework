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

#ifndef TUPLEVARITEM_H
#define TUPLEVARITEM_H

#include "AbstractVarItem.h"

class TupleVarItem : public AbstractVarItem
{
public:
    TupleVarItem();
    QString typeName() const { return "tuple"; }
    ObjectMap setObject(_object *obj);
};

#endif // TUPLEVARITEM_H
