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

#include "DictVarItem.h"
#include <QStringList>
#undef slots
#include "Python.h"

DictVarItem::DictVarItem(bool quote) : mQuote(quote)
{
}

DictVarItem::ObjectMap DictVarItem::setObject(_object *obj)
{
    QMultiMap<QString, ObjectPair> map;
    int size = PyDict_Size(obj);
    mValue = QString("%1 elements").arg(size);
    Py_ssize_t i = 0;
    PyObject* key, *value;
    while (PyDict_Next(obj, &i, &key, &value))
    {
        QString keyStr = PythonTypeConverter::toString(key);
        if(keyStr.isEmpty())// is int of float or other
            keyStr = PythonTypeConverter::toString(key, true);
        else if(mQuote) // is string, quote it
            keyStr = QString("'%1'").arg(keyStr);

        map.insert(keyStr.toLower(), ObjectPair(keyStr, value));
    }
    // the values are sorted by qmap
    return map.values();
}
