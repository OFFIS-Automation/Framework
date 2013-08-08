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

#include "RcUnitInvoker.h"
#include "PythonTypeConverter.h"
#include "PythonProcessSingleton.h"
#undef slots
#include "Python.h"

RcUnitInvoker::RcUnitInvoker()
{
    mNextId = 0;
    connect(comm(), SIGNAL(rcUnitException(int,QString)), this, SLOT(onRcUnitException(int, QString)), Qt::DirectConnection);
    connect(comm(), SIGNAL(rcUnitResult(int,QVariant)), this, SLOT(onRcUnitResult(int, QVariant)), Qt::DirectConnection);
}

RcUnitInvoker &RcUnitInvoker::instance()
{
    static RcUnitInvoker inst;
    return inst;
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
            retVal = invokeRcUnit(QByteArray(unit), QByteArray(method), params);
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
        return PythonTypeConverter::fromVariant(QStringList());
        //@TODO
        //return PythonTypeConverter::fromVariant(RcUnits::instance()->getConstants(QByteArray(unit)));
    }
    catch(const std::exception& )
    {
        QString str = QString("No such unit: %1").arg(unit);
        PyErr_SetString(PyExc_RuntimeError, qPrintable(str));
        return 0;
    }
}

void RcUnitInvoker::rcUnitAnswer(int id, bool error, const QVariant& result)
{
    QMutexLocker lock(&mMutex);
    RcUnitCallState* state = mActiveCalls.value(id, 0);
    if(!state)
        return;
    state->error = error;
    state->result = result;
    state->waiter.wakeAll();
}


QVariant RcUnitInvoker::invokeRcUnit(const QByteArray &unit, const QByteArray &name, const QVariantList &params)
{
    QMutexLocker lock(&mMutex);
    int id = mNextId++;
    comm()->callRcUnit(id, unit, name, params);
    RcUnitCallState state;
    mActiveCalls[id] = &state;
    state.waiter.wait(&mMutex);
    mActiveCalls.remove(id);
    if(state.error)
        throw RcError(state.result.toString());
    return state.result;
}
