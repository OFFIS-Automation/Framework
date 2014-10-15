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

#include "RcUnitsBase.h"
#include "RcUnit.h"

#include <QDebug>
#include <QCoreApplication>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include <QPluginLoader>
#include <lolecs/LolecInterface.h>

#include "telecontrol/TelecontrolFactory.h"
#include "MasterTcInvoker.h"

RcUnitsBase::RcUnitsBase() :
    QObject()
{
    mHaptic = 0;
    mGamepad = 0;
}

RcUnitsBase::~RcUnitsBase()
{
    qDeleteAll(mBaseUnits);
}

RcUnitHelp RcUnitsBase::getHelp(const QString &name)
{
    if(!mBaseUnits.contains(name))
        return RcUnitHelp();
    return mBaseUnits[name]->getHelp();
}

TelecontrolConfig RcUnitsBase::getTelecontrolConfig(const QString &name)
{
    if(mBaseUnits.contains(name))
        return mBaseUnits[name]->telecontrolConfig();
    if(mMasterTcInvokers.contains(name))
        return mMasterTcInvokers[name]->telecontrolConfig();
    return TelecontrolConfig();
}

QList<QString> RcUnitsBase::telecontrolableUnitNames()
{
    QStringList returnList = mMasterTcInvokers.keys();
    foreach(QString name, unitNames())
    {
        if(mUnitsHiddenforTc.contains(name)){
            continue;
        }

        TelecontrolConfig config = getTelecontrolConfig(name);

        bool hasGamepadControl = !(config.tcGamepadButtons.empty() && config.tcGamepadMoves.empty());
        bool hasHapticControl = !(config.tcHapticButtons.empty() && config.tcHapticMoves.empty());
        if(hasGamepadControl || hasHapticControl){
            returnList << name;
        }
    }
    //@TODO add master gamepad controller
    return returnList;
}


QWidget* RcUnitsBase::lolecGui(const QString &name)
{
    if(!mBaseUnits.contains(name))
        return 0;
    LolecInterface *lolecInterface = loadPlugin(mTypes[name]);
    if(lolecInterface)
        return lolecInterface->guiForInstance(mBaseUnits[name]->lolec());
    return 0;
}

void RcUnitsBase::loadConfig(const QString &filename)
{
    releaseConfig();
    QString baseDir = QFileInfo(filename).absolutePath();
    QFile file(filename);
    QSettings settings(filename, QSettings::IniFormat);
    int size = settings.beginReadArray("hilecConfig");
    for(int i=0; i<size; i++)
    {
        settings.setArrayIndex(i);
        QString type = settings.value("type").toString();
        QString name = settings.value("name").toString();
        QString config = settings.value("configFile").toString();
        if(!config.isEmpty())
            config = QFileInfo(baseDir + "/" + config).absoluteFilePath();
        QString error;
        LolecInterface* plugin = loadPlugin(type, &error);
        if(plugin)
        {
            if(plugin->prepareInstantiation())
            {
                RcUnit* rcUnit = new RcUnit(name, config);
                if(rcUnit->initialize(plugin))
                {
                    loadTcSensitivity(name, rcUnit, rcUnit, filename);
                    mBaseUnits[name] = rcUnit;
                    mTypes[name] = type;
                }
                else
                {
                    delete rcUnit;
                    qCritical() << tr("Could not load lolec %1 because lolec type %2 did not create a valid instance, see log information").arg(name, type);
                }
            }
            else
                qCritical() << tr("Could not load lolec %1 because lolec type %2 has not all required data, see log information.").arg(name, type);
        }
        else
            qCritical() << tr("Could not load lolec %1 of type %2: %3").arg(name, type, error);
    }
    settings.endArray();
    loadTcMasters(filename);
    mGamepad = TelecontrolFactory::createGamepad();
    if(mGamepad)
    {
       connect(mGamepad,SIGNAL(buttonToggled(int,bool)), SLOT(onGamepadButtonPressed(int,bool)), Qt::DirectConnection);
       mGamepad->start();
    }
    emit unitsUpdated();
}

void RcUnitsBase::releaseConfig()
{
    mTypes.clear();
    qDeleteAll(mBaseUnits);
    mBaseUnits.clear();
    qDeleteAll(mMasterTcInvokers);
    mMasterTcInvokers.clear();

}

void RcUnitsBase::loadTcMasters(const QString &configFile)
{
    QSettings settings(configFile, QSettings::IniFormat);
    settings.beginGroup("telecontrol-combinations");
    mUnitsHiddenforTc = settings.value("hiddenUnits").toStringList();
    QStringList configs = settings.childGroups();
    foreach (QString configName, configs) {
        MasterTcInvoker* master = new MasterTcInvoker(configName);
        master->readConfig(configFile);
        master->initialize(mBaseUnits.values());
        mMasterTcInvokers[configName] = master;
        loadTcSensitivity(configName, master, 0, configFile);
    }
}

void RcUnitsBase::loadTcSensitivity(const QString& name, GamepadEndpoint *gamepadEndpoint, HapticBaseEndpoint *hapticEndpoint, const QString& configFile)
{
    QSettings settings(configFile, QSettings::IniFormat);
    // Joystick
    settings.beginGroup(QString("joystick/%1").arg(name));
    foreach(QString method, settings.childGroups()){
        double sensitivity = settings.value(QString("%1/sensitivity").arg(method)).toDouble();
        QList<bool> inverts;
        QStringList invertStringList = settings.value(QString("%1/inverts").arg(method)).toStringList();
        foreach(const QString& value, invertStringList){
            inverts << (value.toInt() != 0);
        }
        gamepadEndpoint->updateGamepadSensitivity(method, sensitivity, inverts);
    }

    // Haptics
    if(hapticEndpoint){
        settings.beginGroup(QString("haptic/%1").arg(name));
        foreach(QString method, settings.childGroups()){
            double sensitivity = settings.value(QString("%1/sensitivity").arg(method)).toDouble();
            double forceScaling = settings.value(QString("%1/forceScaling").arg(method)).toDouble();
            QList<bool> inverts;
            QStringList invertStringList = settings.value(QString("%1/inverts").arg(method)).toStringList();
            foreach(const QString& value, invertStringList){
                inverts << (value.toInt() != 0);
            }
            hapticEndpoint->updateHapticSensitivity(method, sensitivity, forceScaling, inverts);
        }
    }
}

LolecInterface* RcUnitsBase::loadPlugin(const QString &type, QString* errMsg)
{
    QStringList names;
    QString found;
    names << type + ".dll" << type + ".dylib" << type + ".so";
    foreach(QString name, names)
    {
        QString path = mLolecDir + "/" + name;
        if(QFileInfo(path).exists())
        {
            found = path;
            break;
        }
    }
    if(found.isEmpty())
    {
        if(errMsg)
            *errMsg = tr("The Plugin file could not be found.");
        return 0;
    }
    QString currentDir = QDir::current().absolutePath();
    QDir::setCurrent(QCoreApplication::applicationDirPath() + "/plugins");
    QPluginLoader loader(found);
    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint);
    QObject* obj = loader.instance();
    QDir::setCurrent(currentDir);
    if(!obj)
    {
        if(errMsg)
            *errMsg = loader.errorString();
        return 0;
    }
    LolecInterface* lolec = qobject_cast<LolecInterface*>(obj);
    if(!lolec)
    {
        if(errMsg)
            *errMsg = tr("The object is no valid LolecInterface. Maybe its outdated?");
    }
    return lolec;
}

QVariant RcUnitsBase::call(const QByteArray &lolec, const QByteArray &method, const QList<QVariant> &params)
{
    if(!mBaseUnits.contains(lolec))
        throw RcError(tr("rc-unit not found: %1").arg(QString(lolec)));
    return mBaseUnits[lolec]->call(method, params);
}

QVariant RcUnitsBase::getConstants(const QByteArray &lolec)
{
    if(!mBaseUnits.contains(lolec))
        throw RcError(tr("rc-unit not found: %1").arg(QString(lolec)));
    return mBaseUnits[lolec]->getConstants();
}

void RcUnitsBase::activateGamepad(const QString &unitName)
{
    if(mGamepad == 0)
    {
        mGamepad = TelecontrolFactory::createGamepad();
        if(!mGamepad)
        {
            qCritical() << tr("No gamepad found.");
            emit telecontrolUpdated(false, QString());
            return;
        }
        // Connect buttons to allow for sensitivity update and unit switch
        connect(mGamepad,SIGNAL(buttonToggled(int,bool)), SLOT(onGamepadButtonPressed(int,bool)), Qt::DirectConnection);
        mGamepad->start();
    }
    GamepadEndpoint* unitToActivate = mMasterTcInvokers.value(unitName, 0);
    if(!unitToActivate)
        unitToActivate = mBaseUnits.value(unitName, 0);

    // Connect old gamepad instances
    foreach(GamepadEndpoint* gamepadEndpoint, mMasterTcInvokers.values()){
        gamepadEndpoint->disconnectGamepad(mGamepad);
    }
    foreach(GamepadEndpoint* gamepadEndpoint, mBaseUnits.values()){
        gamepadEndpoint->disconnectGamepad(mGamepad);
    }

    // Connect the "new" gamepad
    if(unitToActivate){
        unitToActivate->connectGamepad(mGamepad);
    }
    mCurrentTelecontrolledUnit = unitName;
    emit telecontrolUpdated(true, unitName);
}

void RcUnitsBase::deactivateGamepad()
{
    if(mGamepad != 0)
    {
        foreach(RcUnitBase* unit, mBaseUnits.values())
            unit->disconnectGamepad(mGamepad);
        mGamepad->disconnect(this);
        delete mGamepad;
        mGamepad = 0;
    }
    emit telecontrolUpdated(false, QString());
}

void RcUnitsBase::updateGamepad(const QString &unitName, const QString &methodName, double sensitivity, const QList<bool>& inverts)
{
    GamepadEndpoint* unitToUpdate = mMasterTcInvokers.value(unitName, 0);
    if(!unitToUpdate){
        unitToUpdate = mBaseUnits.value(unitName, 0);
    }
    if(!unitToUpdate){
        return;
    }
    unitToUpdate->updateGamepadSensitivity(methodName, sensitivity, inverts);

    QSettings settings(mConfigFile, QSettings::IniFormat);
    settings.beginGroup(QString("telecontrol/%1/%2").arg(unitName, methodName));
    settings.setValue("sensitivity", sensitivity);
    QStringList invertList;
    foreach(bool invert, inverts){
        invertList << QString(invert ? "1" : "0");
    }
    settings.setValue("inverts", invertList);
}

void RcUnitsBase::onGamepadButtonPressed(int buttonId, bool pressed)
{
    if(!pressed || buttonId < Tc::ButtonUp)
        return;
    if(buttonId == Tc::ButtonLeft || buttonId == Tc::ButtonRight)
    {
        emit telecontrolSensitivityChangeRequested(mCurrentTelecontrolledUnit, buttonId == Tc::ButtonRight);
        return;
    }
    QStringList units = telecontrolableUnitNames();
    int currentId = units.indexOf(mCurrentTelecontrolledUnit);
    bool isNewUnit = false;
    if(buttonId == Tc::ButtonUp && currentId > 0)
    {
        currentId--;
        isNewUnit = true;
    }
    else if(buttonId == Tc::ButtonDown && (currentId+1) < units.size())
    {
        currentId++;
        isNewUnit = true;
    }
    QString unit = units.value(currentId, QString());
    if(isNewUnit)
        activateGamepad(unit);
}


void RcUnitsBase::activateHaptic(const QString &unit)
{
    if(mHaptic == 0){
        try
        {
            mHaptic = TelecontrolFactory::createHaptic();
            if(!mHaptic){
                qCritical() << tr("No haptic unit found.");
                emit hapticUpdated(false, QString());
                return;
            }
            mHaptic->connectHardware(mConfigFile);
        }
        catch(const std::exception& e)
        {
            mHaptic = 0;
            qCritical() << "Could not start haptic: " << QString(e.what());
            emit hapticUpdated(false, QString());
            return;
        }
    }
    mHaptic->disable();

    // Connect old haptic interface instances
    foreach(HapticEndpoint* hapticEndpoint, mBaseUnits.values()){
        hapticEndpoint->disconnectHapticDevice(mHaptic);
    }

    // Get the linked RC-Unit
    RcUnitBase* unitToActivate = mBaseUnits.value(unit, 0);

    // Connect the "new" haptic interface
    if(unitToActivate && unitToActivate->hasHapticControl()){
        unitToActivate->connectHapticDevice(mHaptic);
        mHaptic->enable();
    }

    // Store reference, emit signal to notify of connection
    mCurrentHapticUnit = unit;
    emit hapticUpdated(true, unit);
}

void RcUnitsBase::deactivateHaptic()
{
    if(mHaptic){
        mHaptic->disable();
        mHaptic->releaseHardware();
        mHaptic = 0;
    }
    emit hapticUpdated(false, QString());
}

void RcUnitsBase::updateHaptic(const QString &unitName, const QString &methodName, double sensitivity, double forceScaling, const QList<bool>& inverts)
{
    RcUnitBase* unitToUpdate = mBaseUnits.value(unitName, 0);
    if(!unitToUpdate || !unitToUpdate->hasHapticControl()){
        return;
    }
    unitToUpdate->updateHapticSensitivity(methodName, sensitivity, forceScaling, inverts);

    QSettings settings(mConfigFile, QSettings::IniFormat);
    settings.beginGroup(QString("haptic/%1/%2").arg(unitName, methodName));
    settings.setValue("sensitivity", sensitivity);
    settings.setValue("forceScaling", forceScaling);
    QStringList invertList;
    foreach(bool invert, inverts){
        invertList << QString(invert ? "1" : "0");
    }
    settings.setValue("inverts", invertList);
}

QWidget *RcUnitsBase::createHapticWidget()
{
    if(mHaptic == 0){
        return 0;
    }
    return mHaptic->createWidget();
}
