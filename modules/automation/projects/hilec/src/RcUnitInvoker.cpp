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

#include "RcUnitInvoker.h"

#include "RcUnits.h"
#include "PythonTypeConverter.h"

#undef slots
#include "Python.h"

RcUnitInvoker::RcUnitInvoker()
{
}

PyObject* RcUnitInvoker::execute(PyObject* args)
{
    char* unit = "";
    char* method = "";
    PyObject* pyParams = 0;
    if(!PyArg_ParseTuple(args, "ssO", &unit, &method, &pyParams))
    {
        PyErr_SetString(PyExc_RuntimeError, "Wrong number of parameters");
        return 0;
    }
    try
    {
        QList<QVariant> params;
        int len = PyTuple_Size(pyParams);
        for(int i=0; i<len; i++)
        {
            QVariant var = PythonTypeConverter::toVariant(PyTuple_GetItem(pyParams, i));
            params << var;
        }
        QVariant retVal;
        QString errMsg;
        Py_BEGIN_ALLOW_THREADS
        try
        {
            retVal = RcUnits::instance()->call(QByteArray(unit), QByteArray(method), params);
        }
        catch(const std::exception& err)
        {
            errMsg = err.what();
        }
        Py_END_ALLOW_THREADS

        if(!errMsg.isEmpty())
            throw RcError(errMsg);
        PyObject* pyRet = PythonTypeConverter::fromVariant(retVal);
        if(!pyRet)
            throw RcError("Could not parse return value");
        return pyRet;
    }
    catch(const std::exception& err)
    {
        QString str = QString("Error calling <%1::%2>: %3").arg(unit).arg(method).arg(err.what());
        PyErr_SetString(PyExc_RuntimeError, qPrintable(str));
        return 0;
    }

}

PyObject* RcUnitInvoker::getConstants(PyObject *args)
{
    char* unit = "";
    if(!PyArg_ParseTuple(args, "s", &unit))
    {
        PyErr_SetString(PyExc_RuntimeError, "Wrong number of parameters");
        return 0;
    }
    try
    {
        return PythonTypeConverter::fromVariant(RcUnits::instance()->getConstants(QByteArray(unit)));
    }
    catch(const std::exception& )
    {
        QString str = QString("No such unit: %1").arg(unit);
        PyErr_SetString(PyExc_RuntimeError, qPrintable(str));
        return 0;
    }
}
