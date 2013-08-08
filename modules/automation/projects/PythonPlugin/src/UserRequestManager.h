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

#ifndef USERREQUESTMANAGER_H
#define USERREQUESTMANAGER_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QWaitCondition>

struct UserRequest;
struct _object;
class UserRequestParser;

class UserRequestManager : public QObject
{
    Q_OBJECT
public:
    static UserRequestManager& instance();
    int uniqueId();
    _object* execute(_object* args);
signals:
    void newUserRequest(const UserRequest& request);
    void requestAborted(int id);

public slots:
    void abort(_object* unit);
    void abortAll();
    void answer(int id, int button, const QList<QVariant>& data);

protected:
    explicit UserRequestManager();
    static UserRequestManager* sInstance;
    QMutex mMutex;
    QMap<int, UserRequestParser*> mRequests;
    int mNextId;
};

#endif // USERREQUESTMANAGER_H
