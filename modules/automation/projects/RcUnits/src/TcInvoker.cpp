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

#include "TcInvoker.h"
#include <QVector>


TcInvoker::TcInvoker(QObject* device, const QList<RcUnit::TcUpdateMethod>& methods,
                     const QList<RcUnit::TcButtonEvent>& buttons)
    : mDevice(device)
{
    foreach(const RcUnit::TcUpdateMethod& m, methods)
        mMethods[m.deadMansButton] = m;
    foreach(const RcUnit::TcButtonEvent& e, buttons)
        mButtons[e.buttonId] = e;
    mActiveMethod = 0;
}

void TcInvoker::handleData(const QMap<int, double> &data)
{
    if(!mActiveMethod)
        return;
    QVector<QGenericArgument> args(10);
    QVector<double> vals(10);
    QList<Tc::Joystick>& elems = mActiveMethod->joysticks;
    for(int i=0; i<elems.size(); i++)
    {
        int id = elems[i];
        vals[i] = data.value(id, 0.0)*mActiveMethod->sensitivity;
        if(mActiveMethod->inverts[i])
            vals[i] = -vals[i];
        args[i] = Q_ARG(double, vals[i]);
    }
    try
    {
        mActiveMethod->method.invoke(mDevice, Qt::DirectConnection, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
    }
    catch(const std::exception& e)
    {
        qCritical() << tr("Telecontrol update error:") << e.what();
    }
    catch(...)
    {
        qCritical() << tr("Telecontrol update error:") << tr("Unknown");
    }

}

void TcInvoker::setButton(int id, bool pressed)
{
    if(pressed && mMethods.contains(id))
        mActiveMethod = &mMethods[id];
    if(!pressed && mActiveMethod && mActiveMethod->deadMansButton == id)
        mActiveMethod = 0;
    if(mButtons.contains(id))
    {
        RcUnit::TcButtonEvent& event = mButtons[id];
        if(pressed || event.toggleMode)
        {
            try
            {
                event.method.invoke(mDevice,Qt::DirectConnection, Q_ARG(bool, pressed));
            }
            catch(const std::exception& e)
            {
                qCritical() << tr("Telecontrol button error:") << e.what();
            }
            catch(...)
            {
                qCritical() << tr("Telecontrol button error:") << tr("Unknown");
            }
        }
    }
}

void TcInvoker::connectGamepad(QObject *gamepad)
{
    connect(gamepad, SIGNAL(buttonToggled(int,bool)), SLOT(setButton(int,bool)), Qt::DirectConnection);
    connect(gamepad, SIGNAL(dataUpdate(QMap<int,double>)), SLOT(handleData(QMap<int,double>)), Qt::DirectConnection);
}

void TcInvoker::disconnectGamepad(QObject *gamepad)
{
    gamepad->disconnect(this);
    mActiveMethod = 0;
}

void TcInvoker::setSensitivity(const QString &methodName, double sensitivity, const QList<bool>& inverts)
{
    QMutableMapIterator<int, RcUnit::TcUpdateMethod> iter(mMethods);
    while(iter.hasNext())
    {
        RcUnit::TcUpdateMethod& method = iter.next().value();
        if(method.name == methodName)
        {
            method.sensitivity = qMin<double>(sensitivity, 1.0);
            for(int i=0; i<qMin(inverts.size(), method.inverts.size()); i++)
                method.inverts[i] = inverts[i];
            break;
        }
    }
}
