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


#ifndef PYTHONPROCESSCONTROL_H
#define PYTHONPROCESSCONTROL_H

#include "signalProxy/PythonProcessClient.h"
#include <QLocalSocket>
#include <QWaitCondition>
#include <QThreadPool>

class PythonProcessControl : public PythonProcessClient
{
    Q_OBJECT
public:
    explicit PythonProcessControl(QLocalSocket* socket);
    
    QList<TraceLine> getCallStack();

signals:
    
private slots:
    void executeRcUnit(int id, const QByteArray& unit, const QByteArray& method, const QVariantList& params);
    void receiveCallStack(const QList<TraceLine>& trace);
protected:
    QThreadPool mThreadPool;
    QMutex mMutex;
    QWaitCondition mWait;
    QList<TraceLine> mLastCallStack;

};

#endif // PYTHONPROCESSCONTROL_H
