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

#ifndef RCUNIT_H
#define RCUNIT_H

#include <QMap>
#include <QThread>
#include <QMetaMethod>
#include <QMutex>
#include <QWaitCondition>

#include <lolecs/RcStruct.h>
#include <core/RcUnitHelp.h>
#include <lolecs/LolecInterface.h>
#include <lolecs/Pose2d.h>

Q_DECLARE_METATYPE(Pose2d)

#include "RcUnitBase.h"

class TcInvoker;
class RcUnit : public QThread, public RcUnitBase, public RcBase
{
public:

    struct Parameter
    {
        enum Type
        {
            Single,
            List,
            Struct
        } type;
        QByteArray name, realName;
        QString sig;
        int typeId;
        int min, max; // for lists/repeatables
    };

    struct Method : RcUnitHelp::Method
    {
        bool hasReturn;
        Parameter returnType;
        QList<Parameter> arguments;
        bool configured;
        QStringList paramNames;
        QMetaMethod meta;
        Method() { configured = false; }
    };

    struct TcButtonMethod : RcUnitHelp::TcButton
    {
        QMetaMethod method;
        bool hideFromUser;
    };
    struct TcMoveMethod : RcUnitHelp::TcMove
    {
        QMetaMethod method;
        QMap<int, int> invertPos;
    };

    RcUnit(const QString& name, const QString &configFile);
    virtual ~RcUnit();

    QString name() const { return mName; }
    void addMethod(const QString& methodName, const QString& shortDesc, const QString& longDesc);
    void setDesc(const QString &desc) { mDesc = desc; }
    virtual void addConstant(const QString name, const QVariant& constant);
    void setParamNames(const QString &methodName, const QStringList &names);

    bool initialize(LolecInterface* plugin);
    QObject* lolec() { return mLolec; }
    QVariant call(const QByteArray& method, QList<QVariant> params);
    RcUnitHelp getHelp() const;
    void registerStruct(int id, const QByteArray& name, const QStringList& typeNames, const QList<int>& types, RcWrapperFactoryItf* wrapper);

    QVariantMap getConstants() const { return mConstantDefs; }

    QList<TcMoveMethod> tcGamepadMethods() { return mTcGamepadMoveMethods.values(); }
    QList<TcButtonMethod> tcGamepadButtonMethods() { return mTcGamepadButtonMethods; }

    void registerGamepadMethod(QString methodName, const QList<Tc::Joystick>& defaultMapping, int defaultActivateButton, double defaultSensitivity = 1.0/64.0);
    void registerGamepadButtonMethod(QString methodName, int defaultMapping, bool hideFromUser = false);
    void updateGamepadSensitivity(const QString& unitName, double sensitivity, const QList<bool>& inverts);
    void connectGamepad(QObject* gamepad);
    void disconnectGamepad(QObject* gamepad);
    bool hasGamepadControl()const;

    QList<TcMoveMethod> tcHapticMethods() { return mTcHapticMoveMethods.values(); }
    QList<TcButtonMethod> tcHapticButtonMethods() { return mTcHapticButtonMethods; }

    void registerHapticMethod(QString methodName, const QList<Tc::HapticAxis> &defaultMapping, Tc::HapticButton defaultActivateButton, double defaultSensitivity = 1.0/64.0, double defaultForceScaling = 1.0/64.0);
    void registerHapticButtonMethod(QString methodName, Tc::HapticButton defaultMapping, bool hideFromUser = false);
    void updateHapticSensitivity(const QString& unitName, double sensitivity, double forceScaling, const QList<bool>& inverts);
    void connectHapticDevice(QObject* hapticDevice);
    void disconnectHapticDevice(QObject* hapticDevice);
    bool hasHapticControl() const;

protected:
    void run();
    TcInvoker* mTcInvoker;
    void configureRcMethod(const QMetaMethod& method, QString sig);
    void configureGamepadMethod(const QMetaMethod& method, QString sig);
    void configureGamepadButton(TcButtonMethod& buttonMethod, const QMetaMethod& method);
    void configureHapticMethod(const QMetaMethod& method, QString sig);
    void configureHapticButton(TcButtonMethod& buttonMethod, const QMetaMethod& method);

    static QString typeName(QString str);
    static QString typeName(QByteArray str) {return typeName(QString(str)); }
    static QString typeName(const char* str){return typeName(QString(str)); }
    static QString typeName(int type);

    Parameter createParamInfo(QByteArray type, QByteArray name = "");
    QString mName, mConfigFile;
    QString mDesc;
    QObject* mLolec;
    LolecInterface* mLolecInterface;
    QMap<QString, QList<Method>> mMethods;
    QMap<int, RcWrapperFactoryItf*> mWrapperFactories;
    QMap<QString, RcUnitHelp::Struct> mStructDefs;
    QVariantMap mConstantDefs;
    QMap<QString, TcMoveMethod> mTcGamepadMoveMethods;
    QList<TcButtonMethod> mTcGamepadButtonMethods;
    QMap<QString, TcMoveMethod> mTcHapticMoveMethods;
    QList<TcButtonMethod> mTcHapticButtonMethods;
    QMutex mStartMutex;
    QWaitCondition mStartWait;
    QMutex mCallMutex;

};

#endif // RCUNIT_H
