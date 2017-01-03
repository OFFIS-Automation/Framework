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

#include "TcInvoker.h"
#include <QVector>
#include <QMap>
#include <QCoreApplication>
#include <QKeyEvent>


TcInvoker::TcInvoker(QObject* device, const QList<RcUnit::TcMoveMethod>& gamepadMethods, const QList<RcUnit::TcButtonMethod>& gamepadButtonMethods, const QList<RcUnit::TcMoveMethod>& hapticMethods)
    : mDevice(device), mConnexionModifiersPressed(QMap<int, bool>())
{
    // Init joystick method and buttons
    foreach(const RcUnit::TcMoveMethod& gamepadMethod, gamepadMethods){
        mGamepadMethods.append(gamepadMethod);
    }
    foreach(const RcUnit::TcButtonMethod& gamepadButtonMethod, gamepadButtonMethods){
        mGamepadButtonMethods[gamepadButtonMethod.buttonId] = gamepadButtonMethod;
    }

    // Init haptic methods and buttons
    foreach(const RcUnit::TcMoveMethod& hapticMethod, hapticMethods){
        mHapticMethods.append(hapticMethod);
    }
}

// Gamepad stuff
void TcInvoker::connectGamepad(QObject *gamepad)
{
    connect(gamepad, SIGNAL(buttonToggled(int,bool)), SLOT(handleGamepadButtonToggled(int,bool)), Qt::DirectConnection);
    connect(gamepad, SIGNAL(dataUpdated(QMap<int,double>)), SLOT(handleGamepadData(QMap<int,double>)), Qt::DirectConnection);
}

void TcInvoker::disconnectGamepad(QObject *gamepad)
{
    gamepad->disconnect(this);
    mActiveGamepadMethods.clear();
    mCurrentGamepadActivationButton = -1;
}

void TcInvoker::setGamepadParameters(const QString &methodName, double sensitivity, const QList<bool>& inverts)
{
    QMutableListIterator<RcUnit::TcMoveMethod> iter(mGamepadMethods);
    while(iter.hasNext()){
        RcUnit::TcMoveMethod& method = iter.next();
        if(method.name == methodName){
            method.sensitivity = qMin<double>(sensitivity, 1.0);
            for(int i=0; i< qMin(inverts.size(), method.inverts.size()); i++){
                method.inverts[i] = inverts[i];
            }
            break;
        }
    }
}

void TcInvoker::handleGamepadData(const QMap<int, double> &data)
{
    foreach(int activeMethodId, mActiveGamepadMethods){
        RcUnit::TcMoveMethod activeMethod = mGamepadMethods.value(activeMethodId);
        QVector<QGenericArgument> args(10);
        QVector<double> vals(10);
        QList<int>& elems = activeMethod.analogDOFs;
        for(int i=0; i<elems.size(); i++){
            int id = elems[i];
            vals[i] = data.value(id, 0.0)*activeMethod.sensitivity;
            int invertPos = activeMethod.invertPos.value(i, i);
            if(activeMethod.inverts.value(invertPos, false)){
                vals[i] = -vals[i];
            }
            args[i] = Q_ARG(double, vals[i]);
        }
        try
        {
            activeMethod.method.invoke(mDevice, Qt::DirectConnection, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
        }
        catch(const std::exception& e)
        {
            qCritical() << tr("Telecontrol gamepad update error:") << e.what();
        }
        catch(...)
        {
            qCritical() << tr("Telecontrol gamepad update error:") << tr("Unknown");
        }
    }
}

void TcInvoker::handleGamepadButtonToggled(int buttonId, bool pressed)
{
    if(buttonId >= Tc::Connexion::CtrlButton && buttonId <= Tc::Connexion::TabButton){
        mConnexionModifiersPressed[buttonId] = pressed;
        return;
    }
    // Check if one modifier is pressed
    if(mConnexionModifiersPressed.values().contains(true)){
        return;
    }

    if(pressed){
        bool cleared = false;
        for(int i=0; i<mGamepadMethods.size(); i++){
            const RcUnit::TcMoveMethod& method = mGamepadMethods[i];
            if(method.deadMansButton == buttonId && !mActiveGamepadMethods.contains(i)){
                if(!cleared){
                    // Stop current movements
                    handleGamepadData(QMap<int, double>());
                    mActiveGamepadMethods.clear();
                    mCurrentGamepadActivationButton = buttonId;
                    cleared = true;
                }
                mActiveGamepadMethods << i;
            }
        }
    }
    if(!pressed && buttonId == mCurrentGamepadActivationButton){
        // Stop current movements
        handleGamepadData(QMap<int, double>());
        mActiveGamepadMethods.clear();
        mCurrentGamepadActivationButton = -1;
    }
    if(mGamepadButtonMethods.contains(buttonId)){
        RcUnit::TcButtonMethod& event = mGamepadButtonMethods[buttonId];
        if(pressed || event.toggleMode){
            try
            {
                event.method.invoke(mDevice, Qt::DirectConnection, Q_ARG(bool, pressed));
            }
            catch(const std::exception& e)
            {
                qCritical() << tr("Telecontrol gamepad button error:") << e.what();
            }
            catch(...)
            {
                qCritical() << tr("Telecontrol gamepad button error:") << tr("Unknown");
            }
        }
    }
}

// Haptic stuff
void TcInvoker::connectHapticDevice(QObject *hapticDevice)
{
    // Establish signal/slot connection
    connect(hapticDevice, SIGNAL(buttonToggled(int,bool)), SLOT(handleHapticButtonToggled(int,bool)), Qt::DirectConnection);
    connect(hapticDevice, SIGNAL(positionUpdated(QMap<int,double>)), SLOT(handleHapticPositionData(QMap<int,double>)), Qt::DirectConnection);
    connect(this, SIGNAL(forceUpdate(QMap<int,double>)), hapticDevice, SLOT(handleForceUpdate(QMap<int,double>)), Qt::DirectConnection);
}

void TcInvoker::disconnectHapticDevice(QObject *hapticDevice)
{
    // Dissolve signal/slot connection
    hapticDevice->disconnect(this);

    // Reset internal values
    mActiveHapticMethods.clear();
    mCurrentHapticActivationButton = -1;
}

void TcInvoker::setHapticParamaters(const QString& methodName, double sensitivity, double forceScaling, const QList<bool>& inverts)
{
    QMutableListIterator<RcUnit::TcMoveMethod> iter(mHapticMethods);
    while(iter.hasNext()){
        RcUnit::TcMoveMethod& method = iter.next();
        if(method.name == methodName){
            method.sensitivity = qMin<double>(sensitivity, 1.0);
            method.forceScaling = qMin<double>(forceScaling, 1.0);
            for(int i=0; i< qMin(inverts.size(), method.inverts.size()); i++){
                method.inverts[i] = inverts[i];
            }
            break;
        }
    }
}

typedef QMap<int, double> QIntDoubleMap;
void TcInvoker::handleHapticPositionData(const QMap<int,double> &data)
{
    foreach(int activeMethodId, mActiveHapticMethods){
        RcUnit::TcMoveMethod activeMethod = mHapticMethods.value(activeMethodId);
        // Map data to the axes of the active method
        // Required because method may not interested in all axes
        QMap<int,double> argMap;
        if(!data.empty()){
            QList<int>& axesIDs = activeMethod.analogDOFs;
            for(int i=0; i<axesIDs.size(); i++){
                int axesId = axesIDs[i];
                argMap[axesId] = data.value(axesId, 0.0) * (axesId <= Tc::Haptic::AxisZ ? activeMethod.sensitivity : 1.0);
                if(activeMethod.inverts.value(i, false)){
                    argMap[axesId] = -argMap[axesId];
                }
            }
        }

        try
        {
            QIntDoubleMap returnValue;
            qRegisterMetaType<QIntDoubleMap>("QIntDoubleMap");
            activeMethod.method.invoke(mDevice, Qt::DirectConnection, Q_RETURN_ARG(QIntDoubleMap, returnValue), Q_ARG(QIntDoubleMap, argMap));

            QList<int>& axesIDs = activeMethod.analogDOFs;
            for(int i=0; i<axesIDs.size(); i++){
                int axesId = axesIDs[i];
                if(returnValue.contains(axesId)){
                    // Apply scaling and invert
                    returnValue[axesId] *= activeMethod.forceScaling;
                    if(activeMethod.inverts.value(i, false)){
                        returnValue[axesId] = -returnValue[axesId];
                    }
                }
            }
            emit forceUpdate(returnValue);
        }
        catch(const std::exception& e)
        {
            qCritical() << tr("Telecontrol haptic update error:") << e.what();
        }
        catch(...)
        {
            qCritical() << tr("Telecontrol haptic update error:") << tr("Unknown");
        }
    }
}

void TcInvoker::handleHapticButtonToggled(int buttonId, bool pressed)
{
    if(pressed){
        bool cleared = false;
        for(int i=0; i<mHapticMethods.size(); i++){
            const RcUnit::TcMoveMethod& method = mHapticMethods[i];
            if(method.deadMansButton == buttonId){
                if(!cleared){
                    // Stop current movements
                    handleHapticPositionData(QMap<int, double>());
                    mActiveHapticMethods.clear();
                    mCurrentHapticActivationButton = buttonId;
                    cleared = true;
                }
                mActiveHapticMethods << i;
            }
        }
    }
    if(!pressed && buttonId == mCurrentHapticActivationButton){
        // Stop current movements
        handleHapticPositionData(QMap<int, double>());
        mActiveHapticMethods.clear();
        mCurrentHapticActivationButton = -1;
    }
    if(mGamepadButtonMethods.contains(buttonId)){
        RcUnit::TcButtonMethod& event = mGamepadButtonMethods[buttonId];
        if(pressed || event.toggleMode){
            try
            {
                event.method.invoke(mDevice, Qt::DirectConnection, Q_ARG(bool, pressed));
            }
            catch(const std::exception& e)
            {
                qCritical() << tr("Telecontrol haptic button error:") << e.what();
            }
            catch(...)
            {
                qCritical() << tr("Telecontrol haptic button error:") << tr("Unknown");
            }
        }
    }
}
