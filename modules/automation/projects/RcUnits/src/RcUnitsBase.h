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

#ifndef RCUNITSBASE_H
#define RCUNITSBASE_H

#include <QObject>
#include "RcUnitsGlobal.h"
#include <core/RcUnitHelp.h>
#include <rc/RcExceptions.h>


class Gamepad;
class RcUnitInterface;
class RcUnitBase;
class MasterTcInvoker;
class GamepadEndpoint;
class HapticBaseEndpoint;
class HapticDevice;

class RCUNITS_EXPORT RcUnitsBase : public QObject
{
    Q_OBJECT
public:
    explicit RcUnitsBase(const QString& rcUnitPluginDir);
    virtual ~RcUnitsBase();

    RcUnitHelp getHelp(const QString &unitName);
    QMap<QString, HapticDevice *> getHapticDevices();
    TelecontrolConfig getTelecontrolConfig(const QString& name);
    QList<QString> unitNames() { return mUnits.keys(); }
    QList<QString> telecontrolableUnitNames();
    QWidget * rcUnitGui(const QString &name);
    virtual void loadConfig(const QString &filename);
    virtual void releaseConfig();
    QVariant call(const QByteArray &rcUnit, const QByteArray &method, const QList<QVariant> &params);
    QVariant getConstants(const QByteArray& rcUnit);

    void updateTelecontrolAssignment(const QString& unitName, const QString& telecontrolDeviceName);

    void activateGamepad(const QString& unitName);
    void deactivateGamepad(const QString& unitName);
    void deactivateGamepadAll();
    void updateGamepadParameters(const QString& unitName, const QString& methodName, double sensitivity, const QList<bool>& inverts);

    void activateHaptic(const QString& unitName);
    void deactivateHaptic(const QString& unitName);
    void deactivateHapticAll();
    void updateHapticParameters(const QString& unitName, const QString &methodName, double sensitivity, double forceScaling, const QList<bool> &inverts);

    QWidget* createHapticWidget(const QString &unitName);

    void acquire(const QString& unitName);
    void acquireAll();
    void release(const QString& unitName);
    void releaseAll();
    void stop(const QString& unitName);
signals:
    void unitsUpdated();
    void gamepadUpdated(bool gamepadActive, const QString& controlledUnit);
    void gamepadSensitivityChangeRequested(const QString& unitName, bool increase);
    void gamepadSwitchRequested(const QString& unitName, bool down);
    void hapticUpdated(bool hapticActive, const QString& controlledUnit);

private slots:
    void onGamepadButtonPressed(int buttonId, bool pressed, const QString& gamepadName);

protected:
    void loadTcMasters(const QString& configFile);
    void loadTcSensitivity(const QString &name, GamepadEndpoint *gamepadEndpoint, HapticBaseEndpoint *hapticEndpoint, const QString& configFile);

    RcUnitInterface *loadPlugin(const QString& name, QString *errMsg = 0);
    QMap<QString, RcUnitBase*> mUnits;
    QMap<QString, QString> mTypes;
    QString mRcUnitDir;
    QMap<QString, Gamepad *> mGamepadDevices;
    QMap<QString, QString> mGamepadMapping;
    QStringList mTelecontrolRcUnits;
    QString mConfigFile;

    QMap<QString, HapticDevice *> mHapticDevices;
    QMap<QString, MasterTcInvoker*> mMasterTcInvokers;
    QStringList mUnitsHiddenforTc;
};

#endif // RCUNITSBASE_H
