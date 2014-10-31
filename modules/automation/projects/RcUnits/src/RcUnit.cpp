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

void RcUnit::addMethod(const QString &methodName, const QString &shortDesc, const QString &longDesc)
{
    Method m;
    m.name = methodName;
    m.shortDesc = shortDesc;
    m.longDesc = longDesc;
    m.configured = false;
    mMethods[methodName].append(m);
}

void RcUnit::setParamNames(const QString &methodName, const QStringList &names)
{
    if(mMethods.contains(methodName)){
        QList<Method>& methods = mMethods[methodName];
        for(int i=0;i<methods.size();i++){
            Method& method = methods[i];
            if(method.paramNames.size() == names.size()){
                method.paramNames = names;
            }
        }
    }
    if(mTcGamepadMoveMethods.contains(methodName)){
        mTcGamepadMoveMethods[methodName].axeNames = names;
    }
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
        foreach(const Method& m, mMethods[name]){
            help.methods << m;
        }
    }

    names = mStructDefs.keys();
    names.sort();
    foreach(const QString& name, names){
        help.structs << mStructDefs[name];
    }
    help.constants = mConstantDefs;

    // Joystick / gamepad
    names = mTcGamepadMoveMethods.keys();
    names.sort();
    foreach(const QString& name, names){
        help.tcGamepadMoves << mTcGamepadMoveMethods[name];
    }
    foreach(const TcButtonMethod& button, mTcGamepadButtonMethods){
        if(!button.hideFromUser){
            help.tcGamepadButtons << button;
        }
    }

    // Haptic
    names = mTcHapticMoveMethods.keys();
    names.sort();
    foreach(const QString& name, names){
        help.tcHapticMoves << mTcHapticMoveMethods[name];
    }
    foreach(const TcButtonMethod& button, mTcHapticButtonMethods){
        if(!button.hideFromUser){
            help.tcHapticButtons << button;
        }
    }
    help.tcHapticDeviceName = mHapticInterfaceName;

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

    // Wire up methods
    const QMetaObject* meta = mLolec->metaObject();
    for(int i = 0; i<meta->methodCount(); i++){
        QMetaMethod method = meta->method(i);
        // Get method signature
#if QT_VERSION < 0x050000
        QString sig = method.signature();
#else
        QString sig = method.methodSignature();
#endif
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
                configureGamepadButton(buttonEvent, method);
            }
        }
        if(mTcHapticMoveMethods.contains(sig)){
            configureHapticMethod(method, sig);
        }
        for(int i=0;i<mTcHapticButtonMethods.size();i++){
            TcButtonMethod& buttonEvent = mTcHapticButtonMethods[i];
            if(buttonEvent.name == sig){
                configureHapticButton(buttonEvent, method);
            }
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

    // Create a TcInvoker with the created telecontrol methods (for both gamepad and haptic)
    if(mLolec){
        mTcInvoker = new TcInvoker(mLolec, mTcGamepadMoveMethods.values(), mTcGamepadButtonMethods, mTcHapticMoveMethods.values(), mTcHapticButtonMethods);
    }
    return true;
}

void RcUnit::configureRcMethod(const QMetaMethod &method, QString sig)
{
    QList<Method>& methods = mMethods[sig];
    if(methods.last().configured)
        addMethod(methods.last().name, methods.last().shortDesc, methods.last().longDesc);

    Method& m = methods.last();
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

void RcUnit::configureGamepadMethod(const QMetaMethod &method, QString sig)
{
    try
    {
        TcMoveMethod &gamepadMethod = mTcGamepadMoveMethods[sig];
        QList<QByteArray> types = method.parameterTypes();
        if(types.size() != gamepadMethod.joysticks.size()){
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

void RcUnit::configureGamepadButton(TcButtonMethod &buttonMethod, const QMetaMethod &method)
{
    try
    {
        QList<QByteArray> types = method.parameterTypes();
        buttonMethod.toggleMode = types.size() > 0;
        if(buttonMethod.toggleMode && (types.size() > 1 || types[0] != "bool")){
            throw std::runtime_error(tr("Method must have exacly one parameter: %1").arg("(bool enable)").toStdString());
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

void RcUnit::configureHapticButton(TcButtonMethod& buttonMethod, const QMetaMethod& method)
{
    try
    {
        QList<QByteArray> types = method.parameterTypes();
        buttonMethod.toggleMode = types.size() > 0;
        if(buttonMethod.toggleMode && (types.size() > 1 || types[0] != "bool")){
            throw std::runtime_error(tr("Method must have exacly one parameter: %1").arg("(bool enable)").toStdString());
        }
        buttonMethod.method = method;
    }
    catch(const std::exception& e)
    {
        qCritical() << "RcUnit:" << mName << ": " << tr("Could not setup telecontrol joystick button event") << buttonMethod.name << ":" << e.what();
    }
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
                invoker.execute(mLolec);
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

void RcUnit::registerGamepadMethod(QString name, const QList<Tc::Joystick> &defaultMapping, int defaultActivateButton, double defaultSensitivity)
{
    TcMoveMethod method;
    method.name = name;
    method.joysticks = defaultMapping;
    for(int i=0; i<defaultMapping.size(); i++){
        method.inverts << false;
    }
    method.deadMansButton = defaultActivateButton;
    method.sensitivity = defaultSensitivity;
    mTcGamepadMoveMethods[name] = method;
}

void RcUnit::registerGamepadButtonMethod(QString name, int defaultMapping, bool hideFromUser)
{
    TcButtonMethod ev;
    ev.name = name;
    ev.buttonId = defaultMapping;
    ev.hideFromUser = hideFromUser;
    mTcGamepadButtonMethods << ev;
}

void RcUnit::updateGamepadSensitivity(const QString &unitName, double sensitivity, const QList<bool>& inverts)
{
    if(mTcInvoker){
        mTcInvoker->setGamepadSensitivity(unitName, sensitivity, inverts);
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
    }
}

bool RcUnit::hasGamepadControl() const
{
    return !mTcGamepadMoveMethods.empty() || !mTcGamepadButtonMethods.empty();
}

void RcUnit::registerHapticMethod(QString methodName, const QList<Tc::HapticAxis> &defaultMapping, Tc::HapticButton defaultActivateButton, double defaultSensitivity, double defaultForceScaling)
{
    TcMoveMethod method;
    method.name = methodName;
    method.axes = defaultMapping;
    for(int i=0; i<defaultMapping.size(); i++){
        QString axesName;
        switch (defaultMapping.value(i)) {
            case Tc::HapticAxisX:
                axesName = "x";
                break;
            case Tc::HapticAxisY:
                axesName = "y";
                break;
            case Tc::HapticAxisZ:
                axesName = "z";
                break;
            case Tc::HapticAxisYaw:
                axesName = "yaw";
                break;
            case Tc::HapticAxisPitch:
                axesName = "pitch";
                break;
            case Tc::HapticAxisRoll:
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

void RcUnit::registerHapticButtonMethod(QString methodName, Tc::HapticButton defaultMapping, bool hideFromUser)
{
    TcButtonMethod buttonEvent;
    buttonEvent.name = methodName;
    buttonEvent.buttonId = defaultMapping;
    buttonEvent.hideFromUser = hideFromUser;
    mTcHapticButtonMethods << buttonEvent;
}

void RcUnit::updateHapticParameters(const QString& methodName, double sensitivity, double forceScaling, const QList<bool>& inverts)
{
    if(mTcInvoker)
        mTcInvoker->setHapticSensitivity(methodName, sensitivity, forceScaling, inverts);
    if(mTcHapticMoveMethods.contains(methodName))
    {
        TcMoveMethod& method = mTcHapticMoveMethods[methodName];
        method.sensitivity = sensitivity;
        method.forceScaling = forceScaling;
        method.inverts = inverts;
    }
}

void RcUnit::updateHapticAssignment(const QString &hapticInterfaceName)
{
    mHapticInterfaceName = hapticInterfaceName;
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
    return !mTcHapticMoveMethods.empty() || ! mTcHapticButtonMethods.empty();
}

