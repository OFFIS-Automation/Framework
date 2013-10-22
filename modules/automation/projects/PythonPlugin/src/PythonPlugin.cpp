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

#include "PythonPlugin.h"
#include <QDebug>
#include <QFileInfo>
#include <QStringList>
#include "UserRequestManager.h"
#include "RcUnitInvoker.h"
#include "PythonTypeConverter.h"
#include <core/ScriptException.h>

#include "PythonProcessSingleton.h"
#include "PythonDebugger.h"
#include <QCoreApplication>
extern "C"
{
    PyObject* apy_startup(PyObject*, PyObject* args)
    {
        int numParams = 0;
        static QCoreApplication app(numParams, 0);
        PyObject* str = 0;
        PyArg_ParseTuple(args,"O",&str);
        PythonDebugger::instance()->resume();
        if(!PythonProcessSingleton::instance().connectToServer(PythonTypeConverter::toString(str)))
            Py_RETURN_FALSE;
        PyEval_SetTrace(PythonDebugger_Trace, 0);
        Py_RETURN_TRUE;
    }

    PyObject* apy_print(PyObject* , PyObject* args)
    {
        PyObject* str = 0;
        PyArg_ParseTuple(args,"O",&str);

        comm()->printText(PythonTypeConverter::toString(str));
        Py_RETURN_NONE;
    }

    PyObject* apy_printErr(PyObject* , PyObject* args)
    {
        PyObject* str = 0;
        PyArg_ParseTuple(args,"O",&str);
        comm()->printError(PythonTypeConverter::toString(str));
        Py_RETURN_NONE;
    }

    PyObject* apy_userRequest(PyObject* , PyObject* args)
    {
        return UserRequestManager::instance().execute(args);
    }

    PyObject* apy_abortUserRequest(PyObject* , PyObject* args)
    {
        PyObject* elem = 0;
        PyArg_ParseTuple(args, "O", &elem);
        UserRequestManager::instance().abort(elem);
        Py_RETURN_NONE;
    }

    PyObject* apy_createProgress(PyObject*, PyObject* args)
    {
        PyObject* str = 0;
        long maximum;
        PyArg_ParseTuple(args,"Ol",&str, &maximum);
        int id = UserRequestManager::instance().uniqueId();
        comm()->createProgress(id, PythonTypeConverter::toString(str), maximum);
        return PyLong_FromLong(id);
    }

    PyObject* apy_updateProgress(PyObject*, PyObject* args)
    {
        long id, progress = 0;
        PyArg_ParseTuple(args,"ll",&id, &progress);
        comm()->updateProgress(id, progress);
        Py_RETURN_NONE;
    }

    PyObject* apy_removeProgress(PyObject*, PyObject* args)
    {
        long id;
        PyArg_ParseTuple(args,"l",&id);
        comm()->removeProgress(id);
        Py_RETURN_NONE;
    }

    PyObject* apy_createInfoPanel(PyObject*, PyObject* args)
    {
        PyObject* title, *nameList;
        PyArg_ParseTuple(args, "OO", &title, &nameList);
        QStringList names = PythonTypeConverter::toVariant(nameList).toStringList();
        QString name = PythonTypeConverter::toString(title);
        int id = UserRequestManager::instance().uniqueId();
        comm()->createInfoPanel(id, name, names);
        return PyLong_FromLong(id);
    }

    PyObject* apy_updateInfoPanel(PyObject*, PyObject* args)
    {
        PyObject* valueList;
        long id;
        if(!PyArg_ParseTuple(args, "lO", &id, &valueList))
            Py_RETURN_NONE;
        QStringList values = PythonTypeConverter::toVariant(valueList).toStringList();
        comm()->updateInfoPanel(id, values);
        Py_RETURN_NONE;
    }

    PyObject* apy_removeInfoPanel(PyObject*, PyObject* args)
    {
        long id;
        PyArg_ParseTuple(args,"l",&id);
        comm()->removeInfoPanel(id);
        Py_RETURN_NONE;
    }

    PyObject* apy_appendInfo(PyObject*, PyObject* args)
    {
        PyObject* str;
        PyArg_ParseTuple(args,"O",&str);
        comm()->appendInfo(PythonTypeConverter::toString(str));
        Py_RETURN_NONE;
    }

    PyObject* apy_clearInfo(PyObject*, PyObject*)
    {
        comm()->clearInfo();
        Py_RETURN_NONE;
    }

    PyObject* apy_exception(PyObject*, PyObject* args)
    {
        PyObject* name = 0;
        PyObject* stack = 0;
        PyArg_ParseTuple(args, "OO", &name, &stack);
        ScriptException err;
        QString baseDir;//@TODO = HilecCore::instance().baseDir();
        err.name = PythonTypeConverter::toString(name);
        err.name.remove(baseDir);
        err.baseDir = baseDir;
        int size = PyList_Size(stack);
        for(int i=size-1; i>=0; i--)
        {

            PyObject* stackItem = PyList_GetItem(stack, i);
            char* file = "<unknown>";
            char* module = "<unknown>";
            char* method = "<unknown>";
            long line = -1;
            PyArg_ParseTuple(stackItem, "slss", &file, &line, &module, &method);
            ScriptException::Trace trace;
            trace.file = file;
            trace.line = line;
            trace.module = module;
            trace.method = method;
            err.trace.append(trace);
        }
        comm()->raiseException(err);
        Py_RETURN_NONE;
    }

    PyObject* apy_rcCall(PyObject*, PyObject* args)
    {
        return RcUnitInvoker::instance().execute(args);
    }

    PyObject* apy_rcGetConstants(PyObject*, PyObject* args)
    {
        return RcUnitInvoker::instance().getConstants(args);
    }

    PyObject* apy_startVideoCapture(PyObject*, PyObject* args)
    {
        long fps = 0;
        PyArg_ParseTuple(args, "l", &fps);
        HilecCore::instance().startVideoCapture(fps);
        Py_RETURN_NONE;
    }


    PyObject* apy_endVideoCapture(PyObject*, PyObject* args)
    {
        PyObject* name = 0;
        PyArg_ParseTuple(args, "O", &name);
        QString filename = PythonTypeConverter::toString(name, true);
        if(!filename.isEmpty())
            filename = QFileInfo(filename).absoluteFilePath();
        HilecCore::instance().endVideoCapture(filename);
        Py_RETURN_NONE;
    }

    static PyMethodDef io_methods[] = {
            {"startup", apy_startup, METH_VARARGS,""},
            {"log",apy_print, METH_VARARGS,""},
            {"logErr",apy_printErr, METH_VARARGS,""},
            {"userRequest",apy_userRequest, METH_VARARGS,""},
            {"abortUserRequest",apy_abortUserRequest, METH_VARARGS,""},
            {"createProgressBar",apy_createProgress, METH_VARARGS,""},
            {"updateProgressBar",apy_updateProgress, METH_VARARGS,""},
            {"removeProgressBar",apy_removeProgress, METH_VARARGS,""},

            {"createInfoPanel",apy_createInfoPanel, METH_VARARGS,""},
            {"updateInfoPanel",apy_updateInfoPanel, METH_VARARGS,""},
            {"removeInfoPanel",apy_removeInfoPanel, METH_VARARGS,""},
            {"clearInfo",apy_clearInfo, METH_VARARGS,""},
            {"appendInfo",apy_appendInfo, METH_VARARGS,""},
            {"exception",apy_exception, METH_VARARGS,""},
            {"rcCall",apy_rcCall, METH_VARARGS,""},
            {"rcGetConstants",apy_rcGetConstants, METH_VARARGS,""},
            {"startVideoCapture", apy_startVideoCapture, METH_VARARGS, ""},
            {"endVideoCapture", apy_endVideoCapture, METH_VARARGS, ""},
            {NULL, NULL, 0, NULL}
    };

    static struct PyModuleDef offisio = {
       PyModuleDef_HEAD_INIT,
       "offisio",   /* name of module */
       0, /* module documentation, may be NULL */
       -1,       /* size of per-interpreter state of the module,
                    or -1 ilogf the module keeps state in global variables. */
       io_methods
    };
}
PyMODINIT_FUNC PyInit_offisio()
{
   return PyModule_Create(&offisio);
}



