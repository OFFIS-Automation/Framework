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

#include "RcUnit.h"
#include <QMetaMethod>
#include <QMetaObject>
#include <QVector>
#include <QDebug>
#include <QStringList>
#include <QRectF>
#include <QVector3D>
#include <QVector4D>
#include <lolecs/RcExceptions.h>
#include <QDebug>
#include "TcInvoker.h"
#include "RcMethodInvoker.h"
#include <QDebug>

RcUnit::RcUnit(const QString &name, const QString& configFile)
    : mName(name),
      mConfigFile(configFile)

{
    mWrapperFactories[QVariant::String] = new RcWrapperFactory<QString>("QString");
    mWrapperFactories[QVariant::Int] = new RcWrapperFactory<int>("int");
    mWrapperFactories[QMetaType::Float] = new RcWrapperFactory<float>("float");
    mWrapperFactories[QVariant::Double] = new RcWrapperFactory<double>("double");
    mWrapperFactories[QVariant::Bool] = new RcWrapperFactory<bool>("bool");
    mWrapperFactories[QVariant::PointF] = new RcWrapperFactory<QPointF>("QPointF");
    mWrapperFactories[QVariant::SizeF] = new RcWrapperFactory<QSizeF>("QPointF");
    mWrapperFactories[QVariant::RectF] = new RcWrapperFactory<QRectF>("QRectF");
    mWrapperFactories[QVariant::Vector3D] = new RcWrapperFactory<QVector3D>("QVector3D");
    mWrapperFactories[QVariant::Vector4D] = new RcWrapperFactory<QVector4D>("QVector4D");
    mWrapperFactories[QMetaType::QVariant] = new RcWrapperFactory<QVariant>("QVariant");
    int type = QMetaType::type("Pose2d");
    mWrapperFactories[type] = new RcWrapperFactory<Pose2d>("Pose2d");
    setObjectName("RcUnit: " + name);
    mTcInvoker = 0;
    mLolec = 0;
    mHapticSensitivity = 1.0/64.0;
    mHapticForceFactor = 1.0/64.0;
    start();
}

RcUnit::~RcUnit()
{
    quit();
    wait();
    qDeleteAll(mWrapperFactories);
    if(mTcInvoker)
        delete mTcInvoker;

}

void RcUnit::run()
{
    {
        QMutexLocker lock(&mStartMutex);
        mLolec = mLolecInterface->instance(*this, mConfigFile, mName);
        mStartWait.wakeAll();
    }
    if(mLolec)
    {
        setDesc(mLolecInterface->description());
        exec();
        if(mLolec)
            mLolecInterface->deleteInstance(mLolec);
    }
}

void RcUnit::addMethod(const QString &name, const QString &shortDesc, const QString &longDesc)
{
    Method m;
    m.name = name;
    m.shortDesc = shortDesc;
    m.longDesc = longDesc;
    m.configured = false;
    mMethods[name] = m;
}

void RcUnit::setParamNames(const QString &methodName, const QStringList &names)
{
    if(mMethods.contains(methodName))
        mMethods[methodName].paramNames = names;
    if(mTcMethods.contains(methodName))
        mTcMethods[methodName].axeNames = names;
}

RcUnitHelp RcUnit::getHelp() const
{
    RcUnitHelp help;
    help.unitName = name();
    help.desc = mDesc;
    help.server = tr("local", "Server location");
    QStringList names = mMethods.keys();
    names.sort();
    foreach(const QString& name, names)
    {
        const Method& m = mMethods[name];
        if(m.configured)
            help.methods << m;
    }

    names = mStructDefs.keys();
    names.sort();
    foreach(const QString& name, names)
    {
        help.structs << mStructDefs[name];
    }

    names = mTcMethods.keys();
    names.sort();
    foreach(const QString& name, names)
    {
        help.tcJoysticks << mTcMethods[name];
    }

    foreach(const TcButtonEvent& button, mTcButtons)
    {
        if(!button.hideFromUser)
            help.tcButtons << button;
    }
    help.constants = mConstantDefs;
    help.hasHaptic = hasHapticInterface();
    help.hapticSensitivity = hapticSensitivity();
    help.hapticForceFactor = hapticForceFactor();
    return help;
}

bool RcUnit::initialize(LolecInterface* plugin)
{
    QMutexLocker lock(&mStartMutex);
    mLolec = 0;
    mLolecInterface = plugin;
    start();
    mStartWait.wait(&mStartMutex);
    if(!mLolec)
        return false;
    const QMetaObject* meta = mLolec->metaObject();
    for(int i = 0; i<meta->methodCount(); i++)
    {
        QMetaMethod method = meta->method(i);
#if QT_VERSION < 0x050000
        QString sig = method.signature();
#else
        QString sig = method.methodSignature();
#endif
        int end = sig.indexOf('(');
        sig = sig.mid(0,end);
        if(mMethods.contains(sig))
            configureRcMethod(method, sig);
        if(mTcMethods.contains(sig))
            configureTcMethod(method, sig);
        for(int i=0;i<mTcButtons.size();i++)
        {
            TcButtonEvent& buttonEvent = mTcButtons[i];
            if(buttonEvent.name == sig)
            {
                configureTcButton(buttonEvent, method);
            }
        }
        if(sig == mHapticName)
            configureHapticMethod(method);
    }
    foreach(QString unitName, mMethods.keys())
    {
        const Method& method = mMethods[unitName];
        if(!method.configured)
        {
            qCritical() << "Error initializing RC-Unit"  << name() << ": Method" << method.name << "is published, but not found as slot!";
            mMethods.remove(unitName);
        }
    }
    if(mLolec)
        mTcInvoker = new TcInvoker(mLolec, mTcMethods.values(), mTcButtons);
    return true;
}

void RcUnit::configureRcMethod(const QMetaMethod &method, QString sig)
{
    Method &m = mMethods[sig];
    QByteArray retType = method.typeName();
    m.hasReturn = !retType.isEmpty();
    if(m.hasReturn)
        m.returnType = createParamInfo(method.typeName());

    m.sig = sig;
    QStringList params;
    int pos = 0;
    QList<QByteArray> paramNames = method.parameterNames();
    foreach(QByteArray type, method.parameterTypes())
    {
        QByteArray paramName = m.paramNames.value(pos, paramNames[pos]).toLocal8Bit();
        pos++;
        m.arguments << createParamInfo(type, paramName);
        params << m.arguments.last().sig;
    }
    if(m.hasReturn)
        m.sig = m.returnType.sig;
    else
        m.sig = "void";
    m.sig += " " + sig + "(" + params.join(", ") + ")";
    m.meta = method;
    m.configured = true;
}

void RcUnit::configureTcMethod(const QMetaMethod &method, QString sig)
{
    try
    {
        TcUpdateMethod &m = mTcMethods[sig];
        QList<QByteArray> types = method.parameterTypes();
        if(types.size() != m.joysticks.size())
            throw std::runtime_error(tr("Number of mappings is different than number of parameters").toStdString());
        foreach(QByteArray type, types)
        {
            if(type != "double")
                throw std::runtime_error(tr("only double parameter type is supported").toStdString());
        }
        QList<QByteArray> paramNames = method.parameterNames();
        QStringList axeNames;
        for(int i=0;i<paramNames.size();i++)
        {
            axeNames << m.axeNames.value(i, paramNames[i]);
        }
        m.axeNames = axeNames;
        m.method = method;
    }
    catch(const std::exception& e)
    {
        mTcMethods.remove(sig);
        qCritical() << "RcUnit:" << mName << ": " << tr("Could not setup telecontrol method") << sig << ":" << e.what();
    }
}

void RcUnit::configureTcButton(TcButtonEvent &m, const QMetaMethod &method)
{
    try
    {
        QList<QByteArray> types = method.parameterTypes();
        m.toggleMode = types.size() > 0;
        if(m.toggleMode && (types.size() > 1 || types[0] != "bool"))
            throw std::runtime_error(tr("Method must have exacly one parameter: %1").arg("(bool enable)").toStdString());
        m.method = method;
    }
    catch(const std::exception& e)
    {
        qCritical() << "RcUnit:" << mName << ": " << tr("Could not setup telecontrol button event") << m.name << ":" << e.what();
    }
}

void RcUnit::configureHapticMethod(const QMetaMethod &method)
{
    QList<QByteArray> types = method.parameterTypes();
    if(types.size() != 3 || types[0] != "QVector3D&" || types[1] != "QVector3D&" || types[2] != "bool")
    {
        qCritical() << "RcUnit:" << mName << ": " << tr("Could not setup haptic interface method %1 : wrong method signature (see doc)").arg(mHapticName);
        mHapticName.clear();
        return;
    }
    mHapticMethod = method;
}

RcUnit::Parameter RcUnit::createParamInfo(QByteArray origType, QByteArray name)
{
    Parameter p;
    QByteArray type = origType;
    type.replace("RcRepeatable", "QList");

    if(type.contains("QList"))
    {
        p.type = Parameter::List;
        p.min = 0;
        p.max = -1;
        int start = type.indexOf('<') + 1;
        int end = type.indexOf('>');
        type = type.mid(start, end-start);
        QStringList parts = QString(type).split(',', QString::SkipEmptyParts);
        type = parts[0].toLocal8Bit();
        if(parts.size() > 1)
            p.min = qMax(0, parts[1].toInt());
        if(parts.size() > 2)
            p.max = qMax(0, parts[2].toInt());
        p.name = type;
        QString range;
        if(p.min == p.max && p.min != 0)
            range = QString::number(p.min);
        else if(p.max > 0)
            range = QString("%1-%2").arg(p.min).arg(p.max);
        else if(p.max <= 0 && p.min != 0)
            range = QString("%1+").arg(p.min);



        p.sig = QString("%1[%2] %3").arg(typeName(type), range, name).trimmed();
    }
    else
    {
        p.type = Parameter::Single;
        p.name = type;
        p.sig = QString("%1 %2").arg(QString(typeName(type)), QString(name)).trimmed();
    }
    p.realName = origType;
    p.typeId = QMetaType::type(p.name);
    if(p.name.startsWith("Q"))
        p.name = p.name.mid(1);
    if(p.name.endsWith("F"))
        p.name.chop(1);
    return p;
}

QVariant RcUnit::call(const QByteArray &name, QList<QVariant> params)
{
    QVariant value;
    RcMethodInvoker* invoker = 0;
    mCallMutex.lock();
    const Method& m = mMethods.value(name);
    mCallMutex.unlock();
    if(m.configured)
    {
        invoker = new RcMethodInvoker(m, mWrapperFactories);
        invoker->parseArguments(params);
        invoker->execute(mLolec);
        value = invoker->returnValue();
        delete invoker;
    }
    else
    {
        value = mLolecInterface->call(mLolec, name, params);
    }
    return value;
}

void RcUnit::registerStruct(int id, const QByteArray& name, const QStringList& typeNames, const QList<int>& types, RcWrapperFactoryItf* wrapper)
{
    mWrapperFactories[id] = wrapper;
    RcUnitHelp::Struct s;
    s.name = name;
    for(int i=0; i<types.size(); i++)
    {
        QString type = typeName(QMetaType::typeName(types[i]));
        type += " " + typeNames[i];
        s.members << type;
    }
    mStructDefs[name] = s;
}

const HapticResponse RcUnit::currentHapticData()
{
    HapticResponse response;
    hapticMovement(response, true);
    return response;
}

const HapticResponse RcUnit::hapticMovement(const QVector3D &targetPositions)
{
    HapticResponse response;
    response.position.setX(qBound<float>(0.0f, targetPositions.x(), 1.0f));
    response.position.setY(qBound<float>(0.0f, targetPositions.y(), 1.0f));
    response.position.setZ(qBound<float>(0.0f, targetPositions.z(), 1.0f));
    hapticMovement(response, false);
    response.position.setX(qBound<float>(0.0, response.position.x(), 1.0));
    response.position.setY(qBound<float>(0.0, response.position.y(), 1.0));
    response.position.setZ(qBound<float>(0.0, response.position.z(), 1.0));
    response.forces.setX(qBound<float>(0.0, response.forces.x(), 1.0));
    response.forces.setY(qBound<float>(0.0, response.forces.y(), 1.0));
    response.forces.setZ(qBound<float>(0.0, response.forces.z(), 1.0));

    return response;
}

void RcUnit::hapticMovement(HapticResponse &data, bool readOnly)
{

    mHapticMethod.invoke(mLolec, Qt::DirectConnection, Q_ARG(QVector3D, data.position), Q_ARG(QVector3D, data.forces), Q_ARG(bool, readOnly));
}


void RcUnit::addConstant(const QString name, const QVariant &constant)
{
    mConstantDefs[name] = constant;
}


QString RcUnit::typeName(QString str)
{
    str.replace("QVariant", "mixed");
    str.replace("QString", "string");
    if(str.startsWith("Q"))
        str = str.mid(1);
    if(str.length()>1)
        str = str[0] + str.mid(1).toLower();
    return str;
}

void RcUnit::registerGamepadMethod(QString name, const QList<Tc::Joystick> &defaultMapping, int defaultActivateButton, double defaultSensitivity)
{
    TcUpdateMethod method;
    method.name = name;
    method.joysticks = defaultMapping;
    for(int i=0; i<defaultMapping.size(); i++)
        method.inverts << false;
    method.deadMansButton = defaultActivateButton;
    method.sensitivity = defaultSensitivity;
    mTcMethods[name] = method;
}

void RcUnit::registerButtonEvent(QString name, int defaultMapping, bool hideFromUser)
{
    TcButtonEvent ev;
    ev.name = name;
    ev.buttonId = defaultMapping;
    ev.hideFromUser = hideFromUser;
    mTcButtons << ev;
}

void RcUnit::registerHapticMethod(QString methodName)
{
    mHapticName = methodName;
}

void RcUnit::connectGamepad(QObject *gamepad)
{
    if(mTcInvoker)
        mTcInvoker->connectGamepad(gamepad);
}
void RcUnit::disconnectGamepad(QObject *gamepad)
{
    if(mTcInvoker)
        mTcInvoker->disconnectGamepad(gamepad);
}

void RcUnit::updateSensitivity(const QString &unitName, double sensitivity, const QList<bool>& inverts)
{
    if(mTcInvoker)
        mTcInvoker->setSensitivity(unitName, sensitivity, inverts);
    if(mTcMethods.contains(unitName))
    {
        TcUpdateMethod& method = mTcMethods[unitName];
        method.sensitivity = sensitivity;
        method.inverts = inverts;
    }
}


bool RcUnit::isTelecontrolable() const
{
    return !mTcMethods.empty() || ! mTcButtons.empty();
}

