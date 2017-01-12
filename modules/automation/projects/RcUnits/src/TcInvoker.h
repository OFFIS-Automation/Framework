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

#ifndef TCINVOKER_H
#define TCINVOKER_H

#include <QObject>
#include "RcUnit.h"

class TcInvoker : public QObject
{
    Q_OBJECT
public:
    TcInvoker(QObject* device, const QList<RcUnit::TcMoveMethod>& gamepadMethods, const QList<RcUnit::TcButtonMethod>& gamepadButtonMethods, const QList<RcUnit::TcMoveMethod>& hapticMethods);
    void connectGamepad(QObject* gamepad);
    void disconnectGamepad(QObject* gamepad);
    void setGamepadParameters(const QString& methodName, double sensitivity, const QList<bool>& inverts);

    void connectHapticDevice(QObject* hapticDevice);
    void disconnectHapticDevice(QObject* hapticDevice);
    void setHapticParamaters(const QString& methodName, double sensitivity, double forceScaling, const QList<bool>& inverts);
public slots:
    void handleGamepadData(const QMap<int,double>& data);
    void handleGamepadButtonToggled(int buttonId, bool pressed);

    void handleHapticPositionData(const QMap<int,double>& data);
    void handleHapticButtonToggled(int buttonId, bool pressed);

signals:
    /**
     * @brief Send force update.
     * @param data Force data for the axis on a haptic device. Double value should be in range [-1,1].
     */
    void forceUpdate(const QMap<int,double>& data);

protected:
    QList<RcUnit::TcMoveMethod> mGamepadMethods;
    QList<int> mActiveGamepadMethods;
    QMap<int, RcUnit::TcButtonMethod> mGamepadButtonMethods;
    int mCurrentGamepadActivationButton;

    QList<RcUnit::TcMoveMethod> mHapticMethods;
    QList<int> mActiveHapticMethods;
    int mCurrentHapticActivationButton;
    QMap<int, bool> mConnexionModifiersPressed;

    QObject* mDevice;
};

#endif // TCINVOKER_H
