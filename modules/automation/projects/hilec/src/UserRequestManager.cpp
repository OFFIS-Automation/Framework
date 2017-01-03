// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#include "UserRequestManager.h"

#include "UserRequestParser.h"

#undef slots
#include "Python.h"

UserRequestManager* UserRequestManager::sInstance = 0;

UserRequestManager::UserRequestManager() : mNextId(0)
{
    sInstance = this;
}

int UserRequestManager::uniqueId()
{
    QMutexLocker lock(&mMutex);
    return ++mNextId;
}

_object* UserRequestManager::execute(_object *args)
{

    int id = uniqueId();
    try
    {
        UserRequestParser parser(id);
        parser.parseParams(args);
        {
            QMutexLocker lock(&mMutex);
            mRequests[id] = &parser;
        }
        emit newUserRequest(parser.request());
        parser.wait();
        QMutexLocker lock(&mMutex);
        mRequests.remove(id);
        return parser.result();
    }

    catch(const std::exception& err)
    {
        QMutexLocker lock(&mMutex);
        if(mRequests.contains(id))
            mRequests.remove(id);
        QString str = tr("Error executing user request: %1").arg(err.what());
        PyErr_SetString(PyExc_RuntimeError, qPrintable(str));
        return 0;
    }
}

void UserRequestManager::abort(_object *unit)
{
    QMutexLocker lock(&mMutex);
    foreach(UserRequestParser* parser, mRequests.values())
    {
        if(parser->isRequest(unit))
        {
            parser->abort();
            emit requestAborted(parser->request().uid);
            break;
        }
    }
}

void UserRequestManager::abortAll()
{
    QMutexLocker lock(&mMutex);
    foreach(UserRequestParser* parser, mRequests.values())
    {
        parser->abort();
        emit requestAborted(parser->request().uid);
    }
}

void UserRequestManager::answer(int id, int button, const QList<QVariant> &data)
{
    QMutexLocker lock(&mMutex);
    if(!mRequests.contains(id))
        return;
    mRequests[id]->input(button, data);
}
