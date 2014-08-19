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

#include <rc/RcStruct.h>
#include <core/RcUnitHelp.h>
#include <rc/RcUnitInterface.h>
#include <rc/Pose2d.h>

Q_DECLARE_METATYPE(Pose2d)

#include "RcUnitBase.h"

class TcInvoker;
class RcUnit : public QThread, public virtual RcUnitBase, public RcBase
{
    Q_OBJECT
public:
    RcUnit(const QString& name, const QString &configFile);
    virtual ~RcUnit();

    QString name() const { return mName; }
    void addMethod(const QString& name, const QString& shortDesc, const QString& longDesc);
    void setDesc(const QString &desc) { mDesc = desc; }
    virtual void addConstant(const QString name, const QVariant& constant);

    void registerGamepadMethod(QString methodName, const QList<Tc::Joystick>& defaultMapping, int defaultActivateButton, double defaultSensitivity = 1.0/64.0);
    void registerButtonEvent(QString methodName, int defaultMapping, bool hideFromUser = false);
    void connectGamepad(QObject* gamepad);
    void disconnectGamepad(QObject* gamepad);
    void updateSensitivity(const QString& unitName, double sensitivity, const QList<bool>& inverts);
    bool isTelecontrolable()const;
    bool acquired() { return mHwConnected; }
    void setParamNames(const QString &methodName, const QStringList &names);

    bool initialize(RcUnitInterface* plugin);
    UserRcUnit* rcUnit() { return mRcUnit; }
    QVariant call(const QByteArray& method, QList<QVariant> params);
    RcUnitHelp getHelp() const;
    void registerStruct(int id, const QByteArray& name, const QStringList& typeNames, const QList<int>& types, RcWrapperFactoryItf* wrapper);

    QVariantMap getConstants() const { return mConstantDefs; }

    void registerHapticMethod(QString methodName);
    bool hasHapticInterface() const { return !mHapticName.isEmpty(); }
    virtual void updateHapticSensitivity(double sensitivity, double forceFactor) { mHapticSensitivity = sensitivity; mHapticForceFactor = forceFactor; }
    virtual double hapticSensitivity() const { return mHapticSensitivity; }
    virtual double hapticForceFactor() const { return mHapticForceFactor; }
    virtual const HapticResponse currentHapticData();
    virtual const HapticResponse hapticMovement(const QVector3D& targetPositions);
    void hapticMovement(HapticResponse& data, bool readOnly);

    void acquire();// { emit callAcquire(); }
    void release();// { emit callRelease(); }
    void setObserver(RcUnitBaseObserver* observer) { mObserver = observer; }

    QVariantList getFlags();

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

    struct TcUpdateMethod : RcUnitHelp::TcJostick
    {
        QMetaMethod method;
        QMap<int, int> invertPos;
    };
    struct TcButtonEvent : RcUnitHelp::TcButton
    {
        QMetaMethod method;
        bool hideFromUser;
    };
    QList<TcUpdateMethod> tcMethods() { return mTcMethods.values(); }
    QList<TcButtonEvent> tcButtons() { return mTcButtons; }
signals:
    void callAcquire();
    void callRelease();
private slots:
    void hwStatusChanged(bool status);
protected:
    void run();
    TcInvoker* mTcInvoker;
    void configureRcMethod(const QMetaMethod& method, QString sig);
    void configureTcMethod(const QMetaMethod& method, QString sig);
    void configureTcButton(TcButtonEvent& ev, const QMetaMethod& method);
    void configureHapticMethod(const QMetaMethod& method);
    static QString typeName(QString str);
    static QString typeName(QByteArray str) {return typeName(QString(str)); }
    static QString typeName(const char* str){return typeName(QString(str)); }
    static QString typeName(int type);
    Parameter createParamInfo(QByteArray type, QByteArray name = "");
    QString mName, mConfigFile;
    QString mDesc;
    UserRcUnit* mRcUnit;
    RcUnitInterface* mRcUnitInterface;
    QMap<QString, QList<Method>> mMethods;
    QMap<int, RcWrapperFactoryItf*> mWrapperFactories;
    QMap<QString, RcUnitHelp::Struct> mStructDefs;
    QVariantMap mConstantDefs;
    QMap<QString, TcUpdateMethod> mTcMethods;
    QList<TcButtonEvent> mTcButtons;
    QString mHapticName;
    QMetaMethod mHapticMethod;
    QMutex mStartMutex;
    QWaitCondition mStartWait;
    double mHapticSensitivity;
    double mHapticForceFactor;
    QMutex mCallMutex;
    bool mHwConnected;
    RcUnitBaseObserver* mObserver;
};

#endif // RCUNIT_H
