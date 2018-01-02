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

#ifndef DICTVARITEM_H
#define DICTVARITEM_H

#include "AbstractVarItem.h"

class DictVarItem : public AbstractVarItem
{
public:
    DictVarItem(bool quote = false);
    QString typeName() const { return "dict"; }
    ObjectMap setObject(_object *obj);
protected:
    bool mQuote;
};

#endif // DICTVARITEM_H
