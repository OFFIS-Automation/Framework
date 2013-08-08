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

#ifndef RCUNITINVOKER_H
#define RCUNITINVOKER_H

#include <QVariant>
#include <QMutex>
#include <QWaitCondition>

#include <lolecs/RcExceptions.h>

struct _object;

class RcUnitInvoker : public QObject
{
    Q_OBJECT
public:
    static RcUnitInvoker& instance();
    _object* execute(_object* args);
    _object* getConstants(_object* args);
private slots:
    void onRcUnitResult(int id, const QVariant& result) { rcUnitAnswer(id, false, result); }
    void onRcUnitException(int id, const QString& error) { rcUnitAnswer(id, true, error); }
private:
    void rcUnitAnswer(int id, bool error, const QVariant& result);
    QVariant invokeRcUnit(const QByteArray& unit, const QByteArray& name, const QVariantList& params);
    struct RcUnitCallState
    {
        QWaitCondition waiter;
        bool error;
        QVariant result;
    };
    QMap<int, RcUnitCallState*> mActiveCalls;
    QMutex mMutex;
    int mNextId;

    RcUnitInvoker();
};

#endif // RCUNITINVOKER_H
