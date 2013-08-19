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

#ifndef TCINVOKER_H
#define TCINVOKER_H

#include <QObject>
#include "RcUnit.h"

class TcInvoker : public QObject
{
    Q_OBJECT
public:
    TcInvoker(QObject* device, const QList<RcUnit::TcUpdateMethod>& methods, const QList<RcUnit::TcButtonEvent>& buttons);
    void connectGamepad(QObject* gamepad);
    void disconnectGamepad(QObject* gamepad);
    void setSensitivity(const QString& method, double sensitivity, const QList<bool>& inverts);
public slots:
    void handleData(const QMap<int,double>& data);
    void setButton(int id, bool pressed);
protected:
    QMap<int, RcUnit::TcUpdateMethod> mMethods;
    RcUnit::TcUpdateMethod* mActiveMethod;
    QMap<int, RcUnit::TcButtonEvent> mButtons;
    QObject* mDevice;
};

#endif // TCINVOKER_H
