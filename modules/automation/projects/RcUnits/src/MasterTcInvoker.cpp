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


#include "MasterTcInvoker.h"
#include "RcUnit.h"
#include "TcInvoker.h"

#include <QSettings>

MasterTcInvoker::MasterTcInvoker(const QString &name)
{
    mName = name;
}

MasterTcInvoker::~MasterTcInvoker()
{

}

void MasterTcInvoker::readConfig(const QString &configFile)
{
    mConfigFile = configFile;
    QSettings settings(configFile, QSettings::IniFormat);
    settings.beginGroup("telecontrol-combinations");
    settings.beginGroup(mName);
    int size = settings.beginReadArray("joysticks");
    for(int i=0;i<size;i++)
    {
        settings.setArrayIndex(i);
        JoystickWrap wrap;
        wrap.sensitivity = 1/64.0;
        wrap.name = settings.value("name").toString();
        wrap.deadMansButton = Tc::buttonFromString(settings.value("activationButton").toString());
        foreach(Tc::Joystick joystick, Tc::allJoysticks())
        {
            QString groupName = Tc::stringForJoystick(joystick);
            if(settings.childGroups().contains(groupName))
            {
                settings.beginGroup(groupName);
                wrap.joysticks << joystick;
                wrap.axeNames << settings.value("name").toString();
                JoystickWrap::Target target;
                target.unitName = settings.value("unit").toString();
                target.methodName = settings.value("method").toString();
                target.paramName= settings.value("channel").toString();
                wrap.targets << target;
                wrap.inverts << false;
                settings.endGroup();
            }
        }
        mWrappers << wrap;
    }
    settings.endArray();
    size = settings.beginReadArray("buttons");
    for(int i= 0; i< size; i++)
    {
        settings.setArrayIndex(i);
        ButtonWrap wrap;
        wrap.name = settings.value("name").toString();
        wrap.buttonId = Tc::buttonFromString(settings.value("button").toString());
        wrap.targetUnit = settings.value("unit").toString();
        wrap.targetMethod = settings.value("method").toString();
        mButtonWrappers << wrap;
    }
    settings.endArray();

}

void MasterTcInvoker::initialize(QList<RcUnitBase*> units)
{
    tempUnitButtons.clear();
    tempUnitMethods.clear();
    foreach(RcUnitBase* baseUnit, units)
    {
        RcUnit* unit = dynamic_cast<RcUnit*>(baseUnit);
        if(!unit)
            continue;
        foreach (JoystickWrap wrap, mWrappers) {
            setupWrapper(unit, wrap);
        }
        foreach(ButtonWrap wrap, mButtonWrappers)
        {
            if(wrap.targetUnit != unit->name())
                continue;
            foreach(RcUnit::TcButtonEvent event, unit->tcButtons())
            {
                if(event.name == wrap.targetMethod)
                {
                    event.buttonId = wrap.buttonId;
                    tempUnitButtons << event;
                }
            }
        }
        if(!tempUnitButtons.empty() || !tempUnitMethods.empty())
        {
            mInvoker[unit->name()] = new TcInvoker(unit->lolec(), tempUnitMethods, tempUnitButtons);
        }
        tempUnitButtons.clear();
        tempUnitMethods.clear();
    }
}

void MasterTcInvoker::connectGamepad(QObject *gamepad)
{
    foreach(TcInvoker* invoker, mInvoker)
        invoker->connectGamepad(gamepad);
}

void MasterTcInvoker::disconnectGamepad(QObject *gamepad)
{
    foreach(TcInvoker* invoker, mInvoker)
        invoker->disconnectGamepad(gamepad);
}

void MasterTcInvoker::updateSensitivity(const QString &unitName, double sensitivity, const QList<bool> &inverts)
{
    foreach(TcInvoker* invoker, mInvoker)
        invoker->setSensitivity(unitName, sensitivity, inverts);
    for(int i=0;i<mWrappers.size(); i++)
    {
        JoystickWrap& wrap = mWrappers[i];
        if(wrap.name == unitName)
        {
            wrap.sensitivity = sensitivity;
            wrap.inverts = inverts;
        }
    }
}

TelecontrolConfig MasterTcInvoker::telecontrolConfig() const
{
    TelecontrolConfig config;
    foreach(const JoystickWrap& wrap, mWrappers)
        config.tcJoysticks << wrap;
    foreach(const ButtonWrap wrap, mButtonWrappers)
        config.tcButtons << wrap;
    return config;
}

void MasterTcInvoker::setupWrapper(RcUnit* unit, JoystickWrap &wrap)
{
    QString unitName = unit->name();
    QMap<int, int> oldActivationButtons;
    foreach(const RcUnit::TcUpdateMethod& method, unit->tcMethods())
    {
        bool configured = false;
        RcUnit::TcUpdateMethod newMethod = method;
        newMethod.name = wrap.name;
        newMethod.inverts = wrap.inverts;
        for(int i=0;i<newMethod.joysticks.size(); i++)
            newMethod.joysticks[i] = Tc::NoJoystick; // reset all joysticks
        newMethod.deadMansButton = wrap.deadMansButton;

        for(int targetId = 0; targetId < wrap.targets.size(); targetId++)
        {
            JoystickWrap::Target target = wrap.targets[targetId];
            if(target.unitName == unit->name() && target.methodName == method.name)
            {
                for(int paramId = 0; paramId<method.axeNames.size(); paramId++)
                {
                    QString paramName = method.axeNames[paramId];
                    if(paramName == target.paramName)
                    {
                        Tc::Joystick joyId = wrap.joysticks.value(targetId, Tc::NoJoystick);
                        newMethod.joysticks[paramId] = joyId;
                        newMethod.invertPos[paramId] = targetId;
                        newMethod.sensitivity = wrap.sensitivity;
                        configured = true;
                    }
                }
            }
        }
        if(configured)
        {
            oldActivationButtons[method.deadMansButton] = newMethod.deadMansButton;
            tempUnitMethods << newMethod;
        }
    }

    foreach(const RcUnit::TcButtonEvent& event, unit->tcButtons())
    {
        RcUnit::TcButtonEvent newEvent = event;
        // we need to include hidden buttons that are the same as deadManscontrols, even if they are not specified;
        // they might be needed to prepare or stop the movement
        int newbuttonId = oldActivationButtons.value(event.buttonId, -1);
        if(newbuttonId >= 0 && event.hideFromUser)
        {
            // this is such a case
            newEvent.buttonId = newbuttonId;
            tempUnitButtons << newEvent;
        }
    }

}
