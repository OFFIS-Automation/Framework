// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

    QWidget* rcUnitSettingsWidget(const QString &unitName);
    QWidget* rcUnitControlWidget(const QString &unitName);

    virtual void loadConfig(const QString &filename);
    virtual void releaseConfig();
    QVariant call(const QByteArray &rcUnit, const QByteArray &method, const QList<QVariant> &params);
    QVariant getConstants(const QByteArray& rcUnit);

    void updateTelecontrolAssignment(const QString& deviceName, const QString& unitName);

    void activateGamepad(const QString &deviceName, const QString& unitName);
    void deactivateGamepad(const QString& deviceName, const QString& unitName);
    void deactivateGamepadAll();
    void updateGamepadParameters(const QString& unitName, const QString& methodName, double sensitivity, const QList<bool>& inverts);

    void remoteGamepadDataUpdated(const QMap<int, double>& data, const QString& gamepadName);
    void remoteGamepadButtonToggled(int buttonId, const bool& pressed, const QString& gamepadName);

    void activateHaptic(const QString &deviceName, const QString& unitName);
    void deactivateHaptic(const QString& deviceName, const QString& unitName);
    void deactivateHapticAll();
    void updateHapticParameters(const QString& unitName, const QString &methodName, double sensitivity, double forceScaling, const QList<bool> &inverts);

    QWidget* createHapticWidget(const QString &unitName);

    void acquire(const QString& unitName);
    void acquireAll();
    void release(const QString& unitName);
    void releaseAll();
    void stop(const QString& unitName);
    void stopAll();

signals:
    void unitsUpdated();
    void telecontrolAssignmentUpdated(const QString& deviceName, const QString& unitName);
    void gamepadUpdated(const QString& deviceName, const QString& unitName, bool gamepadActive);
    void gamepadSensitivityChangeRequested(const QString& deviceName, const QString& unitName, bool increase);
    void gamepadSwitchRequested(const QString& deviceName, const QString& unitName, bool down);
    void hapticUpdated(const QString& deviceName, const QString& unitName, bool hapticActive);

private slots:
    void onGamepadButtonPressed(int buttonId, bool pressed, const QString& deviceName);

protected:
    void loadTcMasters(const QString& configFile);
    void loadTcSensitivity(const QString &name, GamepadEndpoint *gamepadEndpoint, HapticBaseEndpoint *hapticEndpoint, const QString& configFile);
    bool eventFilter(QObject* watched, QEvent* event);

    RcUnitInterface *loadPlugin(const QString& name, QString *errMsg = 0);
    QMap<QString, RcUnitBase*> mUnits;
    QMap<QString, QString> mTypes;
    QString mRcUnitDir;
    QMap<QString, Gamepad *> mGamepadDevices;
    QMap<QString, QString> mGamepadMapping;
    QStringList mTelecontrolRcUnits;
    QString mConfigFilePath;

    QMap<QString, HapticDevice *> mHapticDevices;
    QMap<QString, MasterTcInvoker*> mMasterTcInvokers;
    QStringList mUnitsHiddenforTc;
    QMap<int, bool> mConnexionModifiersPressed;
};

#endif // RCUNITSBASE_H
