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

RcUnitsBase::RcUnitsBase() :
    QObject()
{
    mHaptic = 0;
    mGamepad = 0;
}

RcUnitsBase::~RcUnitsBase()
{
    qDeleteAll(mUnits);
}

RcUnitHelp RcUnitsBase::getHelp(const QString &name)
{
    if(!mUnits.contains(name))
        return RcUnitHelp();
    return mUnits[name]->getHelp();
}


QWidget* RcUnitsBase::lolecGui(const QString &name)
{
    if(!mUnits.contains(name))
        return 0;
    LolecInterface *lolecInterface = loadPlugin(mTypes[name]);
    if(lolecInterface)
        return lolecInterface->guiForInstance(mUnits[name]->lolec());
    return 0;
}

void RcUnitsBase::loadConfig(const QString &filename)
{
    mTypes.clear();
    qDeleteAll(mUnits);
    QString baseDir = QFileInfo(filename).absolutePath();
    mUnits.clear();
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
                    QSettings teleSettings(filename, QSettings::IniFormat);
                    teleSettings.beginGroup(QString("telecontrol/%1").arg(name));
                    foreach(QString method, teleSettings.childGroups())
                    {
                        double sens = teleSettings.value(QString("%1/sensitivity").arg(method)).toDouble();
                        QList<bool> inverts;
                        QStringList invertStringList = teleSettings.value(QString("%1/inverts").arg(method)).toStringList();
                        foreach(const QString& value, invertStringList)
                            inverts << (value.toInt() != 0);
                        rcUnit->updateSensitivity(method, sens, inverts);
                    }
                    teleSettings.endGroup();
                    double hapticSens = teleSettings.value(QString("haptic/%1/sensitivity").arg(name), rcUnit->hapticSensitivity()).toDouble();
                    double hapticForce = teleSettings.value(QString("haptic/%1/forceFactor").arg(name), rcUnit->hapticForceFactor()).toDouble();
                    rcUnit->updateHapticSensitivity(hapticSens, hapticForce);
                    mUnits[name] = rcUnit;
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
    emit unitsUpdated();
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
    if(!mUnits.contains(lolec))
        throw RcError(tr("rc-unit not found: %1").arg(QString(lolec)));
    return mUnits[lolec]->call(method, params);
}

QVariant RcUnitsBase::getConstants(const QByteArray &lolec)
{
    if(!mUnits.contains(lolec))
        throw RcError(tr("rc-unit not found: %1").arg(QString(lolec)));
    return mUnits[lolec]->getConstants();
}

void RcUnitsBase::activateTelecontrol(const QString &unitName)
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
        connect(mGamepad,SIGNAL(buttonToggled(int,bool)), SLOT(onGamepadButtonPressed(int,bool)), Qt::DirectConnection);
        mGamepad->start();
    }
    RcUnitBase* unitToActivate = mUnits.value(unitName, 0);
    foreach(RcUnitBase* unit, mUnits.values())
        unit->disconnectGamepad(mGamepad);
    if(unitToActivate)
        unitToActivate->connectGamepad(mGamepad);
    mCurrentTelecontrolledUnit = unitName;
    emit telecontrolUpdated(true, unitName);
}

void RcUnitsBase::deactivateTelecontrol()
{
    if(mGamepad != 0)
    {
        foreach(RcUnitBase* unit, mUnits.values())
            unit->disconnectGamepad(mGamepad);
        mGamepad->disconnect(this);
        delete mGamepad;
        mGamepad = 0;
    }
    emit telecontrolUpdated(false, QString());
}

void RcUnitsBase::updateTelecontrol(const QString &unitName, const QString &methodName, double sensitivity, const QList<bool>& inverts)
{
    RcUnitBase* unit = mUnits.value(unitName, 0);
    if(!unit)
        return;
    unit->updateSensitivity(methodName, sensitivity, inverts);

    QSettings settings(mConfigFile, QSettings::IniFormat);
    settings.beginGroup(QString("telecontrol/%1/%2").arg(unitName, methodName));
    settings.setValue("sensitivity", sensitivity);
    QStringList invertList;
    foreach(bool invert, inverts)
        invertList << QString(invert ? "1" : "0");
    settings.setValue("inverts", invertList);
}

void RcUnitsBase::activateHaptic(const QString &unit)
{
    if(!mHaptic)
    {
        try
        {
            mHaptic = TelecontrolFactory::createHaptic();
            if(!mHaptic)
                throw std::runtime_error(tr("No haptic plugin found.").toStdString());
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
    RcUnitBase* unitToActivate = mUnits.value(unit, 0);
    if(unitToActivate && unitToActivate->hasHapticInterface())
    {
        mHaptic->setScaling(unitToActivate->hapticSensitivity(), unitToActivate->hapticForceFactor());
        mHaptic->enable(unitToActivate);
    }
    mCurrentHapticUnit = unit;
    emit hapticUpdated(true, unit);
}

QWidget *RcUnitsBase::createHapticWidget()
{
    if(!mHaptic)
        return 0;
    return mHaptic->createWidget();
}

void RcUnitsBase::deactivateHaptic()
{
    if(mHaptic)
    {
        mHaptic->disable();
        mHaptic->releaseHardware();
        mHaptic = 0;
    }
    emit hapticUpdated(false, QString());
}

void RcUnitsBase::updateHaptic(const QString &unitName, double sensitivity, double forceFactor)
{
    RcUnitBase* unit = mUnits.value(unitName, 0);
    if(!unit)
        return;
    if(mHaptic && unitName == mCurrentHapticUnit)
        mHaptic->setScaling(sensitivity, forceFactor);
    unit->updateHapticSensitivity(sensitivity, forceFactor);
    QSettings settings(mConfigFile, QSettings::IniFormat);
    settings.setValue(QString("haptic/%1/sensitivity").arg(unitName), sensitivity);
    settings.setValue(QString("haptic/%1/forceFactor").arg(unitName), forceFactor);
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
    QStringList& units = mTelecontrolLolecs;
    int currentId = units.indexOf(mCurrentTelecontrolledUnit);
    bool isNewUnit = false;
    if(buttonId == Tc::ButtonUp)
    {
        currentId--;
        isNewUnit = true;
    }
    else if(buttonId == Tc::ButtonDown)
    {
        currentId++;
        isNewUnit = true;
    }
    QString unit = units.value(currentId, QString());
    if(isNewUnit)
        activateTelecontrol(unit);
}
