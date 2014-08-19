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

#ifndef USERREQUESTPARSER_H
#define USERREQUESTPARSER_H

#include <core/UserRequest.h>
#include <QMutex>
#include <QWaitCondition>
#if QT_VERSION < 0x050000
#include <QtGui/QFileDialog>
#else
#include <QtWidgets/QFileDialog>
#endif
struct _object;

class UserRequestParser
{
public:
    UserRequestParser(int id);
    void parseParams(_object* args);
    void input(int buttonId, const QList<QVariant>& data);
    void abort();
    void wait();
    _object* result();
    bool isRequest(_object* obj) { return obj == mObject; }
    const UserRequest& request() { return mRequest; }
protected:
    enum StringMode
    {
        DefaultMode,
        FileMode,
        DirMode
    };

    void parseString(_object* params,int mode = -1);
    void parseInteger(_object* params);
    void parseDouble(_object* params);
    void parseBool(_object* params);
    void parseChoice(_object* params);
    UserRequest mRequest;
    _object* mResult, *mObject;
    QMutex mMutex;
    QWaitCondition mWait;
    bool mFinished, mAborted;
    int mButton;
    QList<QVariant> mData;
};

#endif // USERREQUESTPARSER_H
