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

#include "InstanceVarItem.h"
#undef slots
#include "Python.h"

InstanceVarItem::InstanceVarItem()
{
}

InstanceVarItem::ObjectMap InstanceVarItem::setObject(_object *obj)
{
    ObjectMap children;
    if(PyObject_HasAttrString(obj, "__dict__"))
    {
        PyObject* dict = PyObject_GetAttrString(obj, "__dict__");
        if(PyDict_Check(dict))
        {
            children = DictVarItem::setObject(dict);
        }
        Py_XDECREF(dict);
    }

    PyObject* type = PyObject_Type(obj);
    PyObject* nameObj = PyObject_GetAttrString(type, "__name__");
    QString name = PythonTypeConverter::toString(nameObj);
    PyObject* modObj = PyObject_GetAttrString(type, "__module__");
    QString module = PythonTypeConverter::toString(modObj);
    if(!module.startsWith("__") && module != "builtins")
        name = module + "." + name;
    mType = name;
    Py_XDECREF(modObj);
    Py_XDECREF(nameObj);
    Py_XDECREF(type);

    PyObject* strObj = PyObject_Str(obj);
    mValue = PythonTypeConverter::toString(strObj);
    Py_XDECREF(strObj);


    return children;
}
