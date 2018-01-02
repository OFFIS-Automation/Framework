// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#if 0
#ifndef MASTERTCINVOKER_H
#define MASTERTCINVOKER_H

#include <QObject>
#include <QMap>

#include "GamepadEndpoint.h"
#include "HapticBaseEndpoint.h"
#include "RcUnitsGlobal.h"
#include "RcUnit.h"

class RCUNITS_EXPORT MasterTcInvoker : public QObject, public GamepadEndpoint
{
    Q_OBJECT
public:
    explicit MasterTcInvoker(const QString& name);
    virtual ~MasterTcInvoker();
    void readConfig(const QString& configFile);
    void initialize(QList<RcUnitBase *> units);

    virtual void connectGamepad(QObject* gamepad);
    virtual void disconnectGamepad(QObject* gamepad);
    virtual void updateGamepadParameters(const QString& unitName, double sensitivity, const QList<bool>& inverts);
    virtual void updateGamepadAssignment(const QString& gamepadDeviceName){ Q_UNUSED(gamepadDeviceName) }
    virtual bool hasGamepadControl() const { return true; }

    virtual TelecontrolConfig telecontrolConfig() const;
signals:
    
private slots:

private:

    struct JoystickWrap : TelecontrolConfig::TcMove
    {
        struct Target
        {
            Target(){}
            Target(QString a, QString b, QString c) : unitName(a), methodName(b), paramName(c) {}
            QString unitName;
            QString methodName;
            QString paramName;
        };
        QList<Target> targets;
    };
    struct ButtonWrap : TelecontrolConfig::TcButton
    {
        QString targetUnit;
        QString targetMethod;
    };

    QList<JoystickWrap> mWrappers;
    QList<ButtonWrap> mButtonWrappers;

    QMap<QString, TcInvoker*> mInvoker;
    QList<RcUnit::TcMoveMethod> tempUnitGamepadMethods;
    QList<RcUnit::TcButtonMethod> tempUnitGamepadButtonMethods;

    QString mName, mConfigFile;
    void setupWrapper(RcUnit *unit, JoystickWrap &wrap);

};

#endif // MASTERTCINVOKER_H
#endif
