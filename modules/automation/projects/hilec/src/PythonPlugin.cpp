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

#include "PythonPlugin.h"

#include <QDebug>
#include <QFileInfo>
#include <QStringList>

#include "HilecCore.h"
#include "UserRequestManager.h"
#include "RcUnitInvoker.h"
#include "PythonTypeConverter.h"

#include <core/ScriptException.h>

extern "C"
{
    PyObject* apy_print(PyObject* , PyObject* args)
    {
        PyObject* str = 0;
        PyArg_ParseTuple(args,"O",&str);
        HilecCore::instance().printText(PythonTypeConverter::toString(str));
        Py_RETURN_NONE;
    }

    PyObject* apy_printErr(PyObject* , PyObject* args)
    {
        PyObject* str = 0;
        PyArg_ParseTuple(args,"O",&str);
        HilecCore::instance().printError(PythonTypeConverter::toString(str));
        Py_RETURN_NONE;
    }

    PyObject* apy_userRequest(PyObject* , PyObject* args)
    {
        return UserRequestManager::instance()->execute(args);
    }

    PyObject* apy_abortUserRequest(PyObject* , PyObject* args)
    {
        PyObject* elem = 0;
        PyArg_ParseTuple(args, "O", &elem);
        UserRequestManager::instance()->abort(elem);
        Py_RETURN_NONE;
    }

    PyObject* apy_createProgress(PyObject*, PyObject* args)
    {
        PyObject* str = 0;
        long maximum;
        PyArg_ParseTuple(args,"Ol",&str, &maximum);
        int id = UserRequestManager::instance()->uniqueId();
        HilecCore::instance().createProgress(id, PythonTypeConverter::toString(str), maximum);
        return PyLong_FromLong(id);
    }

    PyObject* apy_updateProgress(PyObject*, PyObject* args)
    {
        long id, progress = 0;
        PyArg_ParseTuple(args,"ll",&id, &progress);
        HilecCore::instance().updateProgress(id, progress);
        Py_RETURN_NONE;
    }

    PyObject* apy_removeProgress(PyObject*, PyObject* args)
    {
        long id;
        PyArg_ParseTuple(args,"l",&id);
        HilecCore::instance().removeProgress(id);
        Py_RETURN_NONE;
    }

    PyObject* apy_createInfoPanel(PyObject*, PyObject* args)
    {
        PyObject* title, *nameList;
        PyArg_ParseTuple(args, "OO", &title, &nameList);
        QStringList names = PythonTypeConverter::toVariant(nameList).toStringList();
        QString name = PythonTypeConverter::toString(title);
        int id = UserRequestManager::instance()->uniqueId();
        HilecCore::instance().createInfoPanel(id, name, names);
        return PyLong_FromLong(id);
    }

    PyObject* apy_updateInfoPanel(PyObject*, PyObject* args)
    {
        PyObject* valueList;
        long id;
        if(!PyArg_ParseTuple(args, "lO", &id, &valueList))
            Py_RETURN_NONE;
        QStringList values = PythonTypeConverter::toVariant(valueList).toStringList();
        HilecCore::instance().updateInfoPanel(id, values);
        Py_RETURN_NONE;
    }

    PyObject* apy_removeInfoPanel(PyObject*, PyObject* args)
    {
        long id;
        PyArg_ParseTuple(args,"l",&id);
        HilecCore::instance().removeInfoPanel(id);
        Py_RETURN_NONE;
    }

    PyObject* apy_appendInfo(PyObject*, PyObject* args)
    {
        PyObject* str;
        PyArg_ParseTuple(args,"O",&str);
        HilecCore::instance().appendInfo(PythonTypeConverter::toString(str));
        Py_RETURN_NONE;
    }

    PyObject* apy_clearInfo(PyObject*, PyObject*)
    {
        HilecCore::instance().clearInfo();
        Py_RETURN_NONE;
    }

    PyObject* apy_exception(PyObject*, PyObject* args)
    {
        PyObject* name = 0;
        PyObject* stack = 0;
        PyArg_ParseTuple(args, "OO", &name, &stack);
        ScriptException err;
        QString baseDir = HilecCore::instance().baseDir();
        err.name = PythonTypeConverter::toString(name);
        err.baseDir = baseDir;

        int size = PyList_Size(stack);
        for(int i=size-1; i>=0; i--)
        {
            PyObject* stackItem = PyList_GetItem(stack, i);
            ScriptException::Trace trace;

            // As of python 3.5, the stack item is no longer a tuple
            // We have to parse it as a FrameSummary Object
            // https://docs.python.org/3/library/traceback.html#traceback.FrameSummary
            // https://github.com/OFFIS-Automation/Framework/issues/32

            PyObject* pFile = PyObject_GetAttrString(stackItem, "filename");
            PyObject* pLineno = PyObject_GetAttrString(stackItem, "lineno");
            PyObject* pName = PyObject_GetAttrString(stackItem, "name");

            trace.file = PythonTypeConverter::toString(pFile);
            trace.line = PythonTypeConverter::toVariant(pLineno).toInt();
            trace.method = PythonTypeConverter::toString(pName);

            if(PyObject_HasAttrString(stackItem, "locals")){
                PyObject* pLocals = PyObject_GetAttrString(stackItem, "locals");
                trace.module = PythonTypeConverter::toString(pLocals);
            } else {
                trace.module = "<unknown>";
            }

            err.trace.append(trace);
        }
        HilecCore::instance().raiseException(err);
        Py_RETURN_NONE;
    }

    PyObject* apy_rcCall(PyObject*, PyObject* args)
    {
        RcUnitInvoker invoker;
        return invoker.execute(args);
    }

    PyObject* apy_rcGetConstants(PyObject*, PyObject* args)
    {
        RcUnitInvoker invoker;
        return invoker.getConstants(args);
    }

    PyObject* apy_startVideoCapture(PyObject*, PyObject* args)
    {
        long fps = 0;
        int widgetIndex = 0;
        PyArg_ParseTuple(args, "l|i", &fps, &widgetIndex);
        HilecCore::instance().startVideoCapture(fps, widgetIndex);
        Py_RETURN_NONE;
    }

    PyObject* apy_endVideoCapture(PyObject*, PyObject* args)
    {
        PyObject* name = 0;
        int widgetIndex = 0;
        PyArg_ParseTuple(args, "O|i", &name, &widgetIndex);
        QString filename = PythonTypeConverter::toString(name, true);
        if(!filename.isEmpty())
            filename = QFileInfo(filename).absoluteFilePath();
        HilecCore::instance().endVideoCapture(filename, widgetIndex);
        Py_RETURN_NONE;
    }

    PyObject* apy_saveScreenshot(PyObject*, PyObject* args)
    {
        PyObject* name = 0;
        int widgetIndex = 0;
        PyArg_ParseTuple(args, "O|i", &name, &widgetIndex);
        QString filename = PythonTypeConverter::toString(name, true);
        if(!filename.isEmpty())
            filename = QFileInfo(filename).absoluteFilePath();
        HilecCore::instance().saveScreenshot(filename, widgetIndex);
        Py_RETURN_NONE;
    }

    static PyMethodDef io_methods[] = {
            {"log", apy_print, METH_VARARGS, ""},
            {"logErr", apy_printErr, METH_VARARGS, ""},
            {"userRequest", apy_userRequest, METH_VARARGS, ""},
            {"abortUserRequest", apy_abortUserRequest, METH_VARARGS, ""},
            {"createProgressBar", apy_createProgress, METH_VARARGS, ""},
            {"updateProgressBar", apy_updateProgress, METH_VARARGS, ""},
            {"removeProgressBar", apy_removeProgress, METH_VARARGS, ""},

            {"createInfoPanel", apy_createInfoPanel, METH_VARARGS, ""},
            {"updateInfoPanel", apy_updateInfoPanel, METH_VARARGS, ""},
            {"removeInfoPanel", apy_removeInfoPanel, METH_VARARGS, ""},
            {"clearInfo", apy_clearInfo, METH_VARARGS, ""},
            {"appendInfo", apy_appendInfo, METH_VARARGS, ""},
            {"exception", apy_exception, METH_VARARGS, ""},
            {"rcCall", apy_rcCall, METH_VARARGS, ""},
            {"rcGetConstants", apy_rcGetConstants, METH_VARARGS, ""},
            {"startVideoCapture", apy_startVideoCapture, METH_VARARGS, ""},
            {"endVideoCapture", apy_endVideoCapture, METH_VARARGS,  ""},
            {"saveScreenshot", apy_saveScreenshot, METH_VARARGS, ""},
            {NULL, NULL, 0, NULL}
    };

    static struct PyModuleDef offisio = {
       PyModuleDef_HEAD_INIT,
       "offisio",   /* name of module */
       0, /* module documentation, may be NULL */
       -1,       /* size of per-interpreter state of the module,
                    or -1 if the module keeps state in global variables. */
       io_methods
    };
}
PyMODINIT_FUNC initIoModule()
{
   return PyModule_Create(&offisio);
}



