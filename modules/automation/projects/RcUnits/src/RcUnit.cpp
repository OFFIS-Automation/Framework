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

#include "RcUnit.h"

#include <QMetaMethod>
#include <QMetaObject>
#include <QStringList>
#include <QRectF>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QDebug>

#include "TcInvoker.h"
#include "RcMethodInvoker.h"

#include <rc/RcExceptions.h>
#include <rc/types/RobotRcUnit.h>

QMutex RcUnit::sNameListMutex;
QMap<QString, QMutex*> RcUnit::sNamedMutexes;

RcUnit::RcUnit(const QString &name, const QString& configFile)
    : mName(name),
      mConfigFile(configFile),
      mObserver(0)

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
    mRcUnit = 0;
    mHwConnected = false;
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
        mRcUnit = mRcUnitInterface->instance(*this, mConfigFile, mName);
        mStartWait.wakeAll();
    }
    if(mRcUnit)
    {
        setDesc(mRcUnitInterface->description());
        if(mRcUnit->rcType() >= HwRcUnitType)
        {
            connect(mRcUnit, SIGNAL(hwConnectionStatusChanged(bool)), this, SLOT(hwStatusChanged(bool)), Qt::QueuedConnection);
            connect(this, SIGNAL(callAcquire()), mRcUnit, SLOT(acquire()), Qt::QueuedConnection);
            connect(this, SIGNAL(callRelease()), mRcUnit, SLOT(release()), Qt::QueuedConnection);
            if(mRcUnit->rcType() == RobotRcUnitType)
                connect(this, SIGNAL(callStop()), mRcUnit, SLOT(stop()), Qt::QueuedConnection);
        }
        exec();
        if(mRcUnit)
            mRcUnitInterface->deleteInstance(mRcUnit);
    }
}

void RcUnit::addMethod(const QString &methodName, const QString &shortDesc, const QString &longDesc, bool hiddenForScratch)
{
    Method m;
    m.hiddenForScratch = hiddenForScratch;
    m.name = methodName;
    m.shortDesc = shortDesc;
    m.longDesc = longDesc;
    m.configured = false;
    mMethods[methodName].append(m);
}

void RcUnit::setParameterNames(const QString &methodName, const QStringList &names)
{
    if(mMethods.contains(methodName)){
        QList<Method>& methods = mMethods[methodName];
        for(int i=0;i<methods.size();i++){
            Method& method = methods[i];
            method.parameterNames = names;
        }
    }
    if(mTcGamepadMoveMethods.contains(methodName)){
        mTcGamepadMoveMethods[methodName].axeNames = names;
    }
}

void RcUnit::setUserInfo(const QString &key, const QVariant &value)
{
    mUserInfo[key] = value;
}

RcUnitHelp RcUnit::getHelp() const
{
    RcUnitHelp help;
    help.unitName = name();
    help.desc = mDesc;
    help.server = tr("local", "Server location");

    QStringList names = mMethods.keys();
    names.sort();
    foreach(const QString& name, names){
        foreach(const Method& method, mMethods[name]){
            help.methods << method;
        }
    }

    names = mStructDefs.keys();
    names.sort();
    foreach(const QString& name, names){
        help.structs << mStructDefs[name];
    }
    help.constants = mConstantDefs;
    help.tcDeviceName = mTelecontrolDeviceName;

    // Joystick / gamepad
    names = mTcGamepadMoveMethods.keys();
    names.sort();
    foreach(const QString& name, names){
        const TcMoveMethod& move = mTcGamepadMoveMethods[name];
        if(move.isConnexion)
            help.tcConnexionMoves << mTcGamepadMoveMethods[name];
        else
            help.tcGamepadMoves << mTcGamepadMoveMethods[name];
    }
    foreach(const TcButtonMethod& button, mTcGamepadButtonMethods){
        if(!button.hideFromUser){
            help.tcButtonMethods << button;
        }
    }

    // Haptic
    names = mTcHapticMoveMethods.keys();
    names.sort();
    foreach(const QString& name, names){
        help.tcHapticMoves << mTcHapticMoveMethods[name];
    }
    help.type = mRcUnit->rcType();
    help.hwConnected = mHwConnected;
    help.flags = mRcUnit->rcFlagDefinitions();
    help.userInfo = mUserInfo;
    return help;
}

bool RcUnit::initialize(RcUnitInterface *plugin)
{
    QMutexLocker lock(&mStartMutex);
    mRcUnit = 0;
    mRcUnitInterface = plugin;
    start();
    mStartWait.wait(&mStartMutex);
    if(!mRcUnit)
        return false;
    // Wire up methods
    const QMetaObject* meta = mRcUnit->metaObject();
    for(int i = 0; i<meta->methodCount(); i++)
    {
        QMetaMethod method = meta->method(i);
        // Get method signature
        QString sig = method.methodSignature();
        int end = sig.indexOf('(');
        sig = sig.mid(0,end);

        if(mMethods.contains(sig)){
            configureRcMethod(method, sig);
        }
        if(mTcGamepadMoveMethods.contains(sig)){
            configureGamepadMethod(method, sig);
        }
        for(int i=0;i<mTcGamepadButtonMethods.size();i++){
            TcButtonMethod& buttonEvent = mTcGamepadButtonMethods[i];
            if(buttonEvent.name == sig){
                configureButtonMethod(buttonEvent, method);
            }
        }
        if(mTcHapticMoveMethods.contains(sig)){
            configureHapticMethod(method, sig);
        }
    }

    // Validate methods
    foreach(QString unitName, mMethods.keys()){
        QList<Method> methods = mMethods[unitName];
        QList<Method> validMethods;
        foreach(const Method& method, methods){
            if(!method.configured){
                qCritical() << "Error initializing RC-Unit"  << name() << ": Method" << method.name << "is published, but not found as slot!";
            } else {
                validMethods << method;
            }
        }
        if(validMethods.empty()){
            mMethods.remove(unitName);
        } else {
            mMethods[unitName] = validMethods;
        }
    }

    if(mRcUnit){
        mTcInvoker = new TcInvoker(mRcUnit, mTcGamepadMoveMethods.values(), mTcGamepadButtonMethods, mTcHapticMoveMethods.values());
    }
    return true;
}

void RcUnit::configureRcMethod(const QMetaMethod &method, QString sig)
{
    QList<Method>& methods = mMethods[sig];
    if(methods.last().configured) // there is already a method, this is overloading
    {
        if(methods.last().parameters.size() == method.parameterTypes().size())
            return; // overloading not possible with the same number of arguments

        QStringList paramNames = methods.last().parameterNames; // copy param names if set by user
        addMethod(methods.last().name, methods.last().shortDesc, methods.last().longDesc);
        methods.last().parameterNames = paramNames;  // copy param names if set by user
    }
    Method& m = methods.last();
    QByteArray retType = method.typeName();
    m.hasReturn = !retType.isEmpty();
    if(m.hasReturn)
        m.returnParameter = createParamInfo(method.typeName());

    m.sig = sig;
    QStringList params;
    int pos = 0;
    QList<QByteArray> paramNames = method.parameterNames();
    foreach(QByteArray type, method.parameterTypes())
    {
        QByteArray paramName = m.parameterNames.value(pos, paramNames[pos]).toLocal8Bit();
        pos++;
        m.parameters << createParamInfo(type, paramName);
        params << m.parameters.last().sig;
    }
    if(m.hasReturn)
        m.sig = m.returnParameter.sig;
    else
        m.sig = "void";
    m.sig += " " + sig + "(" + params.join(", ") + ")";
    m.meta = method;
    m.configured = true;
}

void RcUnit::configureGamepadMethod(const QMetaMethod &method, QString sig)
{
    try
    {
        TcMoveMethod &gamepadMethod = mTcGamepadMoveMethods[sig];
        QList<QByteArray> types = method.parameterTypes();
        if(types.size() != gamepadMethod.analogDOFs.size()){
            throw std::runtime_error(tr("Number of mappings is different than number of parameters").toStdString());
        }
        foreach(QByteArray type, types){
            if(type != "double"){
                throw std::runtime_error(tr("Only double parameter type is supported").toStdString());
            }
        }
        QList<QByteArray> paramNames = method.parameterNames();
        QStringList axeNames;
        for(int i=0;i<paramNames.size();i++){
            axeNames << gamepadMethod.axeNames.value(i, paramNames[i]);
        }
        gamepadMethod.axeNames = axeNames;
        gamepadMethod.method = method;
    }
    catch(const std::exception& e)
    {
        mTcGamepadMoveMethods.remove(sig);
        qCritical() << "RcUnit:" << mName << ": " << tr("Could not setup telecontrol joystick method") << sig << ":" << e.what();
    }
}

void RcUnit::configureButtonMethod(TcButtonMethod &buttonMethod, const QMetaMethod &method)
{
    try
    {
        QList<QByteArray> types = method.parameterTypes();
        buttonMethod.toggleMode = types.size() > 0;
        if(buttonMethod.toggleMode && (types.size() > 1 || types[0] != "bool")){
            throw std::runtime_error(tr("Method must have no or exacly one parameter: %1").arg("(bool enable)").toStdString());
        }
        buttonMethod.method = method;
    }
    catch(const std::exception& e)
    {
        qCritical() << "RcUnit:" << mName << ": " << tr("Could not setup telecontrol joystick button event") << buttonMethod.name << ":" << e.what();
    }
}

void RcUnit::configureHapticMethod(const QMetaMethod& method, QString sig)
{
    try
    {
        TcMoveMethod &hapticMethod = mTcHapticMoveMethods[sig];
        // Check mehtod signature
        if(method.parameterCount() > 1 || method.parameterTypes()[0] != "QMap<int,double>"){
            throw std::runtime_error(tr("Only QMap<int,double> parameter type is supported").toStdString());
        }
        QString methodTypeName = method.typeName();
        if(methodTypeName != "QMap<int,double>"){
            throw std::runtime_error(tr("Only QMap<int,double> return type is supported").toStdString());
        }
        hapticMethod.method = method;
    }
    catch(const std::exception& e)
    {
        mTcHapticMoveMethods.remove(sig);
        qCritical() << "RcUnit:" << mName << ": " << tr("Could not setup telecontrol haptic method") << sig << ":" << e.what();
    }
}


RcUnitHelp::Parameter RcUnit::createParamInfo(QByteArray origType, QByteArray name)
{
    RcUnitHelp::Parameter p;
    QByteArray type = origType;
    type.replace("RcRepeatable", "QList");

    if(type.contains("QList") || type.contains("QVariantList"))
    {
        p.type = RcUnitHelp::Parameter::List;
        p.min = 0;
        p.max = -1;

        if(type.contains("QList")){
            int start = type.indexOf('<') + 1;
            int end = type.indexOf('>');
            type = type.mid(start, end-start);
        } else {
            type = "QVariant";
        }

        QStringList parts = QString(type).split(',', QString::SkipEmptyParts);
        type = parts[0].toLocal8Bit();
        if(parts.size() > 1)
            p.min = qMax(0, parts[1].toInt());
        if(parts.size() > 2)
            p.max = qMax(0, parts[2].toInt());
        p.typeName = type;
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
        p.type = RcUnitHelp::Parameter::Single;
        p.typeName = type;
        p.sig = QString("%1 %2").arg(QString(typeName(type)), QString(name)).trimmed();
    }
    p.realTypeName = origType;
    p.name = name;
    p.typeId = QMetaType::type(p.typeName);
    if(p.typeName.startsWith("Q"))
        p.typeName = p.typeName.mid(1);
    if(p.typeName.endsWith("F"))
        p.typeName.chop(1);
    return p;
}

QVariant RcUnit::call(const QByteArray &name, QList<QVariant> params)
{
    QVariant value;
    mCallMutex.lock();
    const QList<Method>& methods = mMethods.value(name);
    mCallMutex.unlock();
    if(!methods.empty())
    {
        QString lastError;
        foreach(const Method& m, methods)
        {
            RcMethodInvoker invoker(m, mWrapperFactories);
            if(invoker.parseArguments(params, lastError))
            {
                invoker.execute(mRcUnit);
                return invoker.returnValue();
            }
        }
        // if we get here, no method matched the parameters!
        // if there is only one method, return the error of the method
        // else, create new error message
        if(methods.size() > 1)
        {
            QStringList paramTypes;
            foreach(QVariant param, params)
                paramTypes << typeName(param.userType());

            lastError = tr("no matching method found for %1(%2).").arg(name, paramTypes.join(", "));
            lastError += "\n" + tr("Candidates are:");
            foreach(const Method& m, methods)
                lastError += "\n" + m.sig;

        }
        throw RcError(lastError);
    }
    else
    {
        value = mRcUnitInterface->call(mRcUnit, name, params);
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

QMutex* RcUnit::getAcquireMutex(const QString& name)
{
    QMutexLocker lock(&sNameListMutex);
    QMutex* m = sNamedMutexes.value(name, 0);
    if(m == 0)
    {
        m = new QMutex();
        sNamedMutexes[name] = m;
    }
    return m;
}

void RcUnit::acquire()
{
    emit callAcquire();
}

void RcUnit::release()
{
    emit callRelease();
}

void RcUnit::stop()
{
    emit callStop();
}

QVariantList RcUnit::getFlags()
{
    return mRcUnit->rcFlags();
}

void RcUnit::updateTelecontrolAssignment(const QString &telecontrolDeviceName)
{
    mTelecontrolDeviceName = telecontrolDeviceName;
}

void RcUnit::hwStatusChanged(bool status)
{
    mHwConnected = status;
    if(mObserver)
        mObserver->rcUnitStatusChanged(status);
}

void RcUnit::addConstant(const QString name, const QVariant &constant)
{
    mConstantDefs[name] = constant;
}

QString RcUnit::typeName(QString str)
{
    str.replace("QStringList", "string[]");
    str.replace("QVariantList", "mixed[]");
    str.replace("QList", "mixed[]");
    str.replace("QVariant", "mixed");
    str.replace("QString", "string");
    if(str.startsWith("Q"))
        str = str.mid(1);
    if(str.length()>1)
        str = str[0] + str.mid(1).toLower();
    return str;
}

QString RcUnit::typeName(int type)
{
    return typeName(QString(QMetaType::typeName(type)));
}

void RcUnit::registerGamepadMethod(QString name, const QList<Tc::Gamepad::Joystick> &defaultMapping, int defaultActivateButton, double defaultSensitivity, int numSensTicks)
{
    QList<int> mapping;
    foreach(int map, defaultMapping)
        mapping << map;
    registerTelecontrolMethod(name, mapping, defaultActivateButton, defaultSensitivity, numSensTicks, false);
}

void RcUnit::registerConnexionMethod(QString name, const QList<Tc::Connexion::Joystick> &defaultMapping, int defaultActivateButton, double defaultSensitivity, int numSensTicks)
{
    QList<int> mapping;
    foreach(int map, defaultMapping)
        mapping << map;
    registerTelecontrolMethod(name, mapping, defaultActivateButton, defaultSensitivity, numSensTicks, true);
}

void RcUnit::registerTelecontrolMethod(QString name, const QList<int> defaultMapping, int defaultActivateButton, double defaultSensitivity, int numSensTicks, bool connexion)
{
    TcMoveMethod method;
    method.name = name;
    method.numSensitivityScalingTicks = numSensTicks;
    method.analogDOFs = defaultMapping;
    for(int i=0; i<defaultMapping.size(); i++){
        method.inverts << false;
    }
    method.deadMansButton = defaultActivateButton;
    method.sensitivity = defaultSensitivity;
    method.isConnexion = connexion;

    mTcGamepadMoveMethods[name] = method;
}

void RcUnit::registerButtonMethod(QString name, int defaultMapping, bool hideFromUser)
{
    TcButtonMethod ev;
    ev.name = name;
    ev.buttonId = defaultMapping;
    ev.hideFromUser = hideFromUser;
    mTcGamepadButtonMethods << ev;
}

void RcUnit::updateGamepadParameters(const QString &unitName, double sensitivity, const QList<bool>& inverts)
{
    if(mTcInvoker){
        mTcInvoker->setGamepadParameters(unitName, sensitivity, inverts);
    }
    if(mTcGamepadMoveMethods.contains(unitName)){
        TcMoveMethod& method = mTcGamepadMoveMethods[unitName];
        method.sensitivity = sensitivity;
        method.inverts = inverts;
    }
}

void RcUnit::connectGamepad(QObject *gamepad)
{
    if(mTcInvoker){
        mTcInvoker->connectGamepad(gamepad);
    }
}
void RcUnit::disconnectGamepad(QObject *gamepad)
{
    if(mTcInvoker){
        mTcInvoker->disconnectGamepad(gamepad);
        stop();
    }
}

bool RcUnit::hasGamepadControl() const
{
    return !mTcGamepadMoveMethods.empty() || !mTcGamepadButtonMethods.empty();
}

void RcUnit::registerHapticMethod(QString methodName, const QList<Tc::Haptic::Axis> &defaultMapping, int defaultActivateButton, double defaultSensitivity, double defaultForceScaling)
{
    TcMoveMethod method;
    method.name = methodName;
    foreach(int a, defaultMapping)
        method.analogDOFs << a;
    for(int i=0; i<defaultMapping.size(); i++){
        QString axesName;
        switch (defaultMapping.value(i)) {
            // TODO?
            case Tc::Haptic::AxisX:
                axesName = "x";
                break;
            case Tc::Haptic::AxisY:
                axesName = "y";
                break;
            case Tc::Haptic::AxisZ:
                axesName = "z";
                break;
            case Tc::Haptic::AxisYaw:
                axesName = "yaw";
                break;
            case Tc::Haptic::AxisPitch:
                axesName = "pitch";
                break;
            case Tc::Haptic::AxisRoll:
                axesName = "roll";
                break;
            default:
                continue;
        }
        method.axeNames << axesName;
    }
    for(int i=0; i<defaultMapping.size(); i++){
        method.inverts << false;
    }
    method.deadMansButton = defaultActivateButton;
    method.sensitivity = defaultSensitivity;
    method.forceScaling = defaultForceScaling;
    mTcHapticMoveMethods[methodName] = method;
}

void RcUnit::updateHapticParameters(const QString& methodName, double sensitivity, double forceScaling, const QList<bool>& inverts)
{
    if(mTcInvoker)
        mTcInvoker->setHapticParamaters(methodName, sensitivity, forceScaling, inverts);
    if(mTcHapticMoveMethods.contains(methodName))
    {
        TcMoveMethod& method = mTcHapticMoveMethods[methodName];
        method.sensitivity = sensitivity;
        method.forceScaling = forceScaling;
        method.inverts = inverts;
    }
}

void RcUnit::connectHapticDevice(QObject* hapticDevice)
{
    if(mTcInvoker){
        mTcInvoker->connectHapticDevice(hapticDevice);
    }
}

void RcUnit::disconnectHapticDevice(QObject* hapticDevice)
{
    if(mTcInvoker){
        mTcInvoker->disconnectHapticDevice(hapticDevice);
    }
}

bool RcUnit::hasHapticControl()const
{
    return !mTcHapticMoveMethods.empty();
}

