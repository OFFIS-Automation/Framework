// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#include "UserRequestParser.h"
#include "HilecCore.h"
#include "PythonTypeConverter.h"
#include <QVector>
#include <QStringList>
#include <stdexcept>

#undef slots
#include "Python.h"

UserRequestParser::UserRequestParser(int id)
{
    mRequest.uid = id;
    mResult = 0;
    mFinished = false;
    mAborted = false;
}

void UserRequestParser::parseParams(PyObject *args)
{
    PyObject* requests = 0, *buttons = 0, *title = 0;
    PyArg_ParseTuple(args,"OOOO",&mObject, &title, &requests, &buttons);
    mRequest.title =PythonTypeConverter::toString(title);
    int len = PyList_Size(requests);
    for(int i=0; i<len; i++)
    {
        PyObject* request = PyList_GetItem(requests,i);
        PyObject* options = 0;
        char * type = " ";
        PyArg_ParseTuple(request, "sO", &type, &options);
        switch (type[0])
        {
            case 's': parseString(options); break;
            case 'f' : parseString(options, QFileDialog::AnyFile); break;
            case 'F' : parseString(options, QFileDialog::ExistingFile); break;
            case 'D' : parseString(options, QFileDialog::Directory); break;
            case 'i': parseInteger(options); break;
            case 'd': parseDouble(options); break;
            case 'b': parseBool(options); break;
            case 'c': parseChoice(options); break;
        }
    }
    len = PyList_Size(buttons);
    for(int i=0; i<len; i++)
    {
        PyObject* text = 0;
        long id = 0;
        PyObject* pyButton = PyList_GetItem(buttons,i);
        PyArg_ParseTuple(pyButton, "Ol", &text, &id);
        mRequest.buttons.append(UserRequestButton());
        UserRequestButton& button = mRequest.buttons.last();
        button.name = PythonTypeConverter::toString(text);
        button.id = id;
    }
}

void UserRequestParser::wait()
{
    QMutexLocker lock(&mMutex);
    if(!mFinished && !mAborted)
    {
        Py_BEGIN_ALLOW_THREADS
        mWait.wait(&mMutex);
        Py_END_ALLOW_THREADS
    }
    if(mAborted)
        return;

    mResult = PyTuple_New(2);
    PyTuple_SetItem(mResult, 0, PythonTypeConverter::fromVariant(mButton));
    PyTuple_SetItem(mResult, 1, PythonTypeConverter::fromVariant(mData));
}

void UserRequestParser::abort()
{
    QMutexLocker lock(&mMutex);
    mAborted = true;
    mWait.wakeAll();
}


void UserRequestParser::input(int buttonId, const QList<QVariant> &data)
{
    QMutexLocker lock(&mMutex);
    mButton = buttonId;
    mData = data;
    mFinished = true;
    mWait.wakeAll();
}

PyObject* UserRequestParser::result()
{
    if(mResult)
        return mResult;
    else
        Py_RETURN_NONE;
}

void UserRequestParser::parseString(PyObject *params, int mode)
{
    PyObject* desc = 0;
    PyObject* value = 0;
    PyObject* choiceObject;
    PyArg_ParseTuple(params, "OOO", &desc, &value, &choiceObject);
    mRequest.items.append(UserRequestItem());
    UserRequestItem& item = mRequest.items.last();
    item.desc = PythonTypeConverter::toString(desc, true);
    item.value =PythonTypeConverter::toString(value, true);
    QStringList choices = PythonTypeConverter::toVariant(choiceObject).toStringList();
    if(mode >= 0)
        item.constraints["fileMode"] = mode;
    else if(!choices.empty())
        item.constraints["stringChoices"] = choices;
}

void UserRequestParser::parseInteger(PyObject *params)
{
    PyObject* desc = 0;
    long value = 0, minVal = 0, maxVal = 0, stepping = 1;
    PyArg_ParseTuple(params, "Ollll", &desc, &value, &minVal, &maxVal, &stepping);
    mRequest.items.append(UserRequestItem());
    UserRequestItem& item = mRequest.items.last();
    item.desc = PythonTypeConverter::toString(desc, true);
    item.value = (int)value;
    item.constraints["min"] = (int)minVal;
    item.constraints["max"] = (int)maxVal;
    item.constraints["stepping"] = (int)stepping;
}

void UserRequestParser::parseDouble(PyObject *params)
{
    PyObject* desc = 0;
    double value = 0.0;
    PyArg_ParseTuple(params, "Od", &desc, &value);
    mRequest.items.append(UserRequestItem());
    UserRequestItem& item = mRequest.items.last();
    item.desc = PythonTypeConverter::toString(desc);
    item.value = (double)value;
}

void UserRequestParser::parseBool(PyObject *params)
{
    PyObject* desc = 0, *trueName = 0, *falseName = 0;
    PyObject* value;
    PyArg_ParseTuple(params, "OOOO", &desc, &value, &trueName, &falseName);
    mRequest.items.append(UserRequestItem());
    UserRequestItem& item = mRequest.items.last();
    item.desc = PythonTypeConverter::toString(desc, true);
    if(value == Py_True)
        item.value = true;
    else
        item.value = false;
    item.constraints["trueName"] = PythonTypeConverter::toString(trueName);
    item.constraints["falseName"] = PythonTypeConverter::toString(falseName);
}

void UserRequestParser::parseChoice(_object *params)
{
    PyObject* desc = 0, *namesObject = 0, *valuesObject= 0;
    PyObject* defaultValue = 0;
    PyArg_ParseTuple(params, "OOOO", &desc, &namesObject, &valuesObject, &defaultValue);

    QStringList names = PythonTypeConverter::toVariant(namesObject).toStringList();
    QVariantList values = PythonTypeConverter::toVariant(valuesObject).toList();
    if(names.empty())
        throw std::runtime_error("names must be a none empty list of strings");
    if(names.size() != values.size())
            throw std::runtime_error("names and values must be of the same size");
    mRequest.items.append(UserRequestItem());
    UserRequestItem& item = mRequest.items.last();
    item.desc = PythonTypeConverter::toString(desc, true);
    item.value = PythonTypeConverter::toVariant(defaultValue);

    item.constraints["names"] = names;
    item.constraints["choices"] = values;

}

