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

#include "RcUnitsBase.h"
#include "RcUnit.h"

#include <QDebug>
#include <QCoreApplication>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include <QPluginLoader>
#include <rc/RcUnitInterface.h>
#include <QKeyEvent>
#include <QList>
#include "telecontrol/TelecontrolFactory.h"
#include "MasterTcInvoker.h"

RcUnitsBase::RcUnitsBase(const QString &rcUnitPluginDir) :
    QObject(), mConnexionModifiersPressed(QMap<int, bool>())
{
    mRcUnitDir = rcUnitPluginDir;
    qApp->installEventFilter(this);
}

RcUnitsBase::~RcUnitsBase()
{
    qDeleteAll(mUnits);
}

RcUnitHelp RcUnitsBase::getHelp(const QString &unitName)
{
    if(!mUnits.contains(unitName))
        return RcUnitHelp();
    return mUnits[unitName]->getHelp();
}

 QMap<QString, HapticDevice *> RcUnitsBase::getHapticDevices()
{
    return mHapticDevices;
}

TelecontrolConfig RcUnitsBase::getTelecontrolConfig(const QString &name)
{
    if(mUnits.contains(name)){
        return mUnits[name]->telecontrolConfig();
    }
    //@TODO check master tcInvoker
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

        bool hasGamepadControl = !(config.tcButtonMethods.empty() && config.tcGamepadMoves.empty());
        bool hasHapticControl = config.tcHapticMoves.empty();
        if(hasGamepadControl || hasHapticControl){
            returnList << name;
        }
    }
    //@TODO add master gamepad controller
    return returnList;
}


QWidget* RcUnitsBase::rcUnitGui(const QString &name)
{
    if(!mUnits.contains(name))
        return 0;
    RcUnitInterface *rcUnitInterface = loadPlugin(mTypes[name]);
    if(rcUnitInterface)
        return rcUnitInterface->settingsWidgetForInstance(mUnits[name]->rcUnit());
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
        RcUnitInterface* plugin = loadPlugin(type, &error);
        if(plugin)
        {
            if(plugin->prepareInstantiation())
            {
                RcUnit* rcUnit = new RcUnit(name, config);
                if(rcUnit->initialize(plugin))
                {
                    loadTcSensitivity(name, rcUnit, rcUnit, filename);
                    mUnits[name] = rcUnit;
                    mTypes[name] = type;
                }
                else
                {
                    delete rcUnit;
                    qCritical() << tr("Could not load rcUnit %1 because rcUnit type %2 did not create a valid instance, see log information").arg(name, type);
                }
            }
            else
                qCritical() << tr("Could not load rcUnit %1 because rcUnit type %2 has not all required data, see log information.").arg(name, type);
        }
        else
            qCritical() << tr("Could not load rcUnit %1 of type %2: %3").arg(name, type, error);
    }
    settings.endArray();
    loadTcMasters(filename);

    // Connect buttons to allow for sensitivity update and unit switch
    mGamepadDevices = TelecontrolFactory::getGamepadDevices();
    foreach (Gamepad *gamepad, mGamepadDevices) {
        gamepad->disconnect(this);
        connect(gamepad, SIGNAL(buttonToggled(int,QList<int>,bool,QString)), SLOT(onGamepadButtonPressed(int,QList<int>,bool,QString)), Qt::DirectConnection);
        gamepad->start();
    }
    emit unitsUpdated();
}

void RcUnitsBase::releaseConfig()
{
    mTypes.clear();
    qDeleteAll(mUnits);
    mUnits.clear();
    qDeleteAll(mMasterTcInvokers);
    mMasterTcInvokers.clear();

}

void RcUnitsBase::loadTcMasters(const QString &configFile)
{
    /*
     * // TODO?
    QSettings settings(configFile, QSettings::IniFormat);
    settings.beginGroup("telecontrol-combinations");
    mUnitsHiddenforTc = settings.value("hiddenUnits").toStringList();
    QStringList configs = settings.childGroups();
    foreach (QString configName, configs) {
        MasterTcInvoker* master = new MasterTcInvoker(configName);
        master->readConfig(configFile);
        master->initialize(mUnits.values());
        mMasterTcInvokers[configName] = master;
        loadTcSensitivity(configName, master, 0, configFile);
    }
    */
}

void RcUnitsBase::loadTcSensitivity(const QString& name, GamepadEndpoint *gamepadEndpoint, HapticBaseEndpoint *hapticEndpoint, const QString& configFile)
{
    QSettings settings(configFile, QSettings::IniFormat);

    // Joystick
    settings.beginGroup(QString("gamepad/%1").arg(name));
    foreach(QString method, settings.childGroups()){
        double sensitivity = settings.value(QString("%1/sensitivity").arg(method)).toDouble();
        QList<bool> inverts;
        QStringList invertStringList = settings.value(QString("%1/inverts").arg(method)).toStringList();
        foreach(const QString& value, invertStringList){
            inverts << (value.toInt() != 0);
        }
        gamepadEndpoint->updateGamepadParameters(method, sensitivity, inverts);
    }
    settings.endGroup();

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
            hapticEndpoint->updateHapticParameters(method, sensitivity, forceScaling, inverts);
        }
        settings.endGroup();
    }

    settings.beginGroup(QString("telecontrol/%1").arg(name));
    QString telecontrolDeviceName = settings.value("telecontrolDeviceName").toString();
    dynamic_cast<RcUnit *>(gamepadEndpoint)->updateTelecontrolAssignment(telecontrolDeviceName);
    settings.endGroup();
}

bool RcUnitsBase::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* keyEvent = reinterpret_cast<QKeyEvent*>(event);
        if(keyEvent->isAutoRepeat()){
            return false;
        }
        bool pressed = event->type() == QEvent::KeyPress;
        Qt::Key key = (Qt::Key)keyEvent->key();
        int button = -1;
        // handle enables ONLY if the ctrl modifier is there
        // and handle release ALWAYS (otherwise, is shift is released first,
        // we won't capture the event)
        if((keyEvent->modifiers() | Qt::ControlModifier) != 0 || pressed){
            switch(keyEvent->key())
            {
            case Qt::Key_F1:
                button = Tc::Virtual::CtrlF1Button;
                break;
            case Qt::Key_F2:
                button = Tc::Virtual::CtrlF2Button;
                break;
            case Qt::Key_F3:
                button = Tc::Virtual::CtrlF3Button;
                break;
            default:
                return false;
            }
        }
        if(key){
            if(button < 0){
                return false;
            }
            foreach(Gamepad* gamepad, mGamepadDevices.values()){
                gamepad->buttonToggled(button, pressed, gamepad->getName());
            }
            foreach(HapticDevice* hapticDevice, mHapticDevices.values()){
                hapticDevice->buttonToggled(button, pressed);
            }
        }
        return true;
    }
    return false;
}

RcUnitInterface* RcUnitsBase::loadPlugin(const QString &type, QString* errMsg)
{
    QStringList names;
    QString found;
    names << type + ".dll" << type + ".dylib" << type + ".so";
    foreach(QString name, names)
    {
        QString path = mRcUnitDir + "/" + name;
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
    RcUnitInterface* rcUnit = qobject_cast<RcUnitInterface*>(obj);
    if(!rcUnit)
    {
        if(errMsg)
            *errMsg = tr("The object is no valid RcUnitInterface. Maybe its outdated?");
    }
    return rcUnit;
}

QVariant RcUnitsBase::call(const QByteArray &rcUnit, const QByteArray &method, const QList<QVariant> &params)
{
    if(!mUnits.contains(rcUnit))
        throw RcError(tr("rc-unit not found: %1").arg(QString(rcUnit)));
    return mUnits[rcUnit]->call(method, params);
}

QVariant RcUnitsBase::getConstants(const QByteArray &rcUnit)
{
    if(!mUnits.contains(rcUnit))
        throw RcError(tr("rc-unit not found: %1").arg(QString(rcUnit)));
    return mUnits[rcUnit]->getConstants();
}

void RcUnitsBase::updateTelecontrolAssignment(const QString &unitName, const QString &telecontrolDeviceName)
{
    RcUnitBase* unitToUpdate = mUnits.value(unitName, 0);
    if(!unitToUpdate){
        return;
    }

    QSettings settings(mConfigFile, QSettings::IniFormat);
    settings.beginGroup(QString("telecontrol/%1").arg(unitName));
    settings.setValue("telecontrolDeviceName", telecontrolDeviceName);

    if(telecontrolDeviceName.length() > 0){
        if(!mGamepadMapping.contains(telecontrolDeviceName)){
            activateGamepad(unitName);
            activateHaptic(unitName);
        }
    } else {
        deactivateGamepad(unitName);
        deactivateHaptic(unitName);
    }

    // Connet this unit
    unitToUpdate->updateTelecontrolAssignment(telecontrolDeviceName);
    emit telecontrolAssignmentUpdated(unitName, telecontrolDeviceName);

}

void RcUnitsBase::activateGamepad(const QString &unitName)
{
    if(mGamepadDevices.empty()){
        mGamepadDevices = TelecontrolFactory::getGamepadDevices();
        if(mGamepadDevices.empty()){
            qCritical() << tr("No gamepad found.");
            emit gamepadUpdated(false, QString());
            return;
        }
    }

    QString currentTelecontrolDeviceName = getTelecontrolConfig(unitName).tcDeviceName;
    if(mGamepadDevices.keys().contains(currentTelecontrolDeviceName)){
        Gamepad *gamepad = mGamepadDevices[currentTelecontrolDeviceName];

        // Connect buttons to allow for sensitivity update and unit switch
        gamepad->disconnect(this);
        connect(gamepad, SIGNAL(buttonToggled(int,bool,QString)), SLOT(onGamepadButtonPressed(int,bool,QString)), Qt::DirectConnection);
        gamepad->start();

        // Disconnect old gamepad instances
        /*
        //removed master TC invokers
        foreach(GamepadEndpoint* gamepadEndpoint, mMasterTcInvokers.values()){
            gamepadEndpoint->disconnectGamepad(gamepad);
        }
        */
        foreach(GamepadEndpoint* gamepadEndpoint, mUnits.values()){
            gamepadEndpoint->disconnectGamepad(gamepad);
        }

        // Get the linked RC-Unit
        // removed master tc invokers
        GamepadEndpoint* unitToActivate = 0 ;// = mMasterTcInvokers.value(unitName, 0);
        if(!unitToActivate){
            unitToActivate = mUnits.value(unitName, 0);
        }

        // Connect the new gamepad
        if(unitToActivate && unitToActivate->hasGamepadControl()){
            unitToActivate->connectGamepad(gamepad);
        }

        mGamepadMapping[currentTelecontrolDeviceName] = unitName;
    }
    emit gamepadUpdated(true, unitName);
}

void RcUnitsBase::deactivateGamepad(const QString &unitName)
{
    QString currentTelecontrolDeviceName = getTelecontrolConfig(unitName).tcDeviceName;
    if(mGamepadDevices.keys().contains(currentTelecontrolDeviceName)){
         Gamepad *gamepad = mGamepadDevices[currentTelecontrolDeviceName];
         gamepad->disconnect(this);

         RcUnitBase* unitToDeactivate = mUnits.value(unitName, 0);
         unitToDeactivate->disconnectGamepad(gamepad);

         mGamepadMapping.remove(currentTelecontrolDeviceName);
    }
    emit gamepadUpdated(false, unitName);
}

void RcUnitsBase::deactivateGamepadAll()
{
    foreach(RcUnitBase* unit, mUnits.values()){
       QString currentTelecontrolDeviceName = getTelecontrolConfig(unit->name()).tcDeviceName;
       if(mGamepadDevices.keys().contains(currentTelecontrolDeviceName)){
            Gamepad *gamepad = mGamepadDevices[currentTelecontrolDeviceName];
            gamepad->disconnect(this);

            unit->disconnectGamepad(gamepad);

            mGamepadMapping.remove(currentTelecontrolDeviceName);
       }
   }
    emit gamepadUpdated(false, QString());
}

void RcUnitsBase::updateGamepadParameters(const QString &unitName, const QString &methodName, double sensitivity, const QList<bool>& inverts)
{
    // removed master tc invokers
    GamepadEndpoint* unitToUpdate = 0;//mMasterTcInvokers.value(unitName, 0);
    if(!unitToUpdate){
        unitToUpdate = mUnits.value(unitName, 0);
    }
    if(!unitToUpdate){
        return;
    }
    unitToUpdate->updateGamepadParameters(methodName, sensitivity, inverts);

    QSettings settings(mConfigFile, QSettings::IniFormat);
    settings.beginGroup(QString("gamepad/%1/%2").arg(unitName, methodName));
    settings.setValue("sensitivity", sensitivity);
    QStringList invertList;
    foreach(bool invert, inverts){
        invertList << QString(invert ? "1" : "0");
    }
    settings.setValue("inverts", invertList);
}

void RcUnitsBase::acquire(const QString &unitName)
{
    RcUnitBase* unit = mUnits.value(unitName, 0);
    if(!unit)
        return;
    unit->acquire();
}

void RcUnitsBase::release(const QString &unitName)
{
    RcUnitBase* unit = mUnits.value(unitName, 0);
    if(!unit)
        return;
    unit->release();
}

void RcUnitsBase::acquireAll()
{
    foreach(RcUnitBase* unit, mUnits)
    {
        unit->acquire();
    }
}

void RcUnitsBase::releaseAll()
{
    foreach(RcUnitBase* unit, mUnits)
        unit->release();
}

void RcUnitsBase::stop(const QString &unitName)
{
    RcUnitBase* unit = mUnits.value(unitName, 0);
    if(!unit)
        return;
    unit->stop();
}

void RcUnitsBase::stopAll()
{
    foreach(RcUnitBase* unit, mUnits)
        unit->stop();
}


void RcUnitsBase::onGamepadButtonPressed(int buttonId, bool pressed, const QString &gamepadName)
{
    if(buttonId >= Tc::Connexion::MenuButton && buttonId <= Tc::Connexion::FitButton){
        mConnexionModifiersPressed[buttonId] = pressed;
        return;
    }

    if(pressed){
        // Get the connected unit
        QString unitName;
        if(mGamepadMapping.contains(gamepadName)){
            unitName = mGamepadMapping[gamepadName];

            // Gamepad methods
            if(buttonId == Tc::Gamepad::ButtonLeft || buttonId == Tc::Gamepad::ButtonRight){
                emit gamepadSensitivityChangeRequested(unitName, buttonId == Tc::Gamepad::ButtonRight);
            } else if(buttonId == Tc::Gamepad::ButtonDown || buttonId == Tc::Gamepad::ButtonUp){
                emit gamepadSwitchRequested(unitName, buttonId == Tc::Gamepad::ButtonDown);
            }

            // Connexion methods
            if(mConnexionModifiersPressed[Tc::Connexion::MenuButton]){
                if(buttonId == Tc::Connexion::ShiftButton || buttonId == Tc::Connexion::CtrlButton){
                    emit gamepadSensitivityChangeRequested(unitName, buttonId == Tc::Connexion::CtrlButton);
                } else if(buttonId == Tc::Connexion::EscButton || buttonId == Tc::Connexion::AltButton){
                    emit gamepadSwitchRequested(unitName, buttonId == Tc::Connexion::AltButton);
                }
            }
        }
    }


    // Check for interesting button
    if((buttonId >= Tc::Gamepad::ButtonUp && buttonId <= Tc::Gamepad::ButtonRight) || buttonId == Tc::Connexion::Rotate90Button){
        // Get the connected unit
        QString unitName;
        if(mGamepadMapping.contains(gamepadName)){
            unitName = mGamepadMapping[gamepadName];
        }

        if(buttonId == Tc::Gamepad::ButtonLeft || buttonId == Tc::Gamepad::ButtonRight){
            emit gamepadSensitivityChangeRequested(unitName, buttonId == Tc::Gamepad::ButtonRight);
        } else if(buttonId == Tc::Gamepad::ButtonDown || buttonId == Tc::Gamepad::ButtonUp){
            emit gamepadSwitchRequested(unitName, buttonId == Tc::Gamepad::ButtonDown);
        } else if(buttonId == Tc::Connexion::Rotate90Button){
            emit gamepadSwitchRequested(unitName, buttonId == Tc::Gamepad::ButtonDown);
        }
    }
}


void RcUnitsBase::activateHaptic(const QString &unitName)
{
    if(mHapticDevices.empty()){
        mHapticDevices = TelecontrolFactory::getHapticDevices();
        if(mHapticDevices.empty()){
            qCritical() << tr("No haptic units found.");
            emit hapticUpdated(false, QString());
            return;
        }
    }

    QString currentTelecontrolDeviceName = getHelp(unitName).tcDeviceName;
    if(mHapticDevices.keys().contains(currentTelecontrolDeviceName)){
        HapticDevice *hapticDevice = mHapticDevices[currentTelecontrolDeviceName];
        hapticDevice->disable();

        // Disconnect old haptic interface instances
        foreach(HapticEndpoint* hapticEndpoint, mUnits.values()){
            hapticEndpoint->disconnectHapticDevice(hapticDevice);
        }

        // Get the linked RC-Unit
        RcUnitBase* unitToActivate = mUnits.value(unitName, 0);

        // Connect the "new" haptic interface
        if(unitToActivate && unitToActivate->hasHapticControl()){
            unitToActivate->connectHapticDevice(hapticDevice);
            hapticDevice->enable();
        }
    }

    emit hapticUpdated(true, unitName);
}

void RcUnitsBase::deactivateHaptic(const QString &unitName)
{
    QString currentTelecontrolDeviceName = getHelp(unitName).tcDeviceName;
    if(mHapticDevices.keys().contains(currentTelecontrolDeviceName)){
         HapticDevice *hapticDevice = mHapticDevices[currentTelecontrolDeviceName];
         hapticDevice->disable();

         RcUnitBase* unitToDeactivate = mUnits.value(unitName, 0);
         unitToDeactivate->disconnectHapticDevice(hapticDevice);
    }
    emit hapticUpdated(false, unitName);
}

void RcUnitsBase::deactivateHapticAll()
{
    foreach(RcUnitBase* unit, mUnits.values()){
        QString currentTelecontrolDeviceName = getHelp(unit->name()).tcDeviceName;
        if(mHapticDevices.keys().contains(currentTelecontrolDeviceName)){
             HapticDevice *hapticDevice = mHapticDevices[currentTelecontrolDeviceName];
             hapticDevice->disable();
             unit->disconnectHapticDevice(hapticDevice);
        }
        emit hapticUpdated(false, QString());
    }
}

void RcUnitsBase::updateHapticParameters(const QString &unitName, const QString &methodName, double sensitivity, double forceScaling, const QList<bool>& inverts)
{
    RcUnitBase* unitToUpdate = mUnits.value(unitName, 0);
    if(!unitToUpdate || !unitToUpdate->hasHapticControl()){
        return;
    }
    unitToUpdate->updateHapticParameters(methodName, sensitivity, forceScaling, inverts);

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

QWidget *RcUnitsBase::createHapticWidget(const QString &unitName)
{
    if(mHapticDevices.empty()){
        mHapticDevices = TelecontrolFactory::getHapticDevices();
        if(mHapticDevices.empty()){
            qCritical() << tr("No haptic units found.");
            return 0;
        }
    }

    QString currentTelecontrolDeviceName = getHelp(unitName).tcDeviceName;
    if(mHapticDevices.keys().contains(currentTelecontrolDeviceName)){
        HapticDevice *hapticDevice = mHapticDevices[currentTelecontrolDeviceName];
        return hapticDevice->createWidget();
    }

    return 0;
}
