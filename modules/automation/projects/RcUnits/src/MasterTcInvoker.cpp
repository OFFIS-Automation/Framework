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


#include "MasterTcInvoker.h"
#include "RcUnit.h"
#include "TcInvoker.h"

MasterTcInvoker::MasterTcInvoker(const QString &name)
{
    JoystickWrap j1;
    j1.axeNames << "x" << "y" << "slide" << "z";
    j1.targets << JoystickWrap::Target("arm", "moveGamepadLinear", "x");
    j1.targets << JoystickWrap::Target("arm", "moveGamepadLinear", "y");
    j1.targets << JoystickWrap::Target("stepper", "moveSlideG", "duration");
    j1.targets << JoystickWrap::Target("arm", "moveGamepadLinear", "z");
    j1.joysticks << Tc::LeftJoystickX << Tc::LeftJoystickY << Tc::RightJoystickX << Tc::RightJoystickY;
    j1.deadMansButton = Tc::LeftShoulderUpperButton;
    j1.inverts << false << false << false << false;
    j1.name  = "moveSlide";
    j1.sensitivity = 1/64.0;
    mWrappers << j1;

    ButtonWrap b1;
    b1.name = "CloseGripper";
    b1.buttonId = Tc::LeftShoulderLowerButton;
    b1.toggleMode = false;
    b1.targetMethod = "CloseGripper";
    b1.targetUnit = "gripper";
    mButtonWrappers << b1;
}

MasterTcInvoker::~MasterTcInvoker()
{

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
    {
        connect(gamepad, SIGNAL(buttonToggled(int,bool)), invoker, SLOT(setButton(int,bool)), Qt::DirectConnection);
        connect(gamepad, SIGNAL(dataUpdate(QMap<int,double>)), invoker, SLOT(handleData(QMap<int,double>)), Qt::DirectConnection);
    }
}

void MasterTcInvoker::disconnectGamepad(QObject *gamepad)
{
    foreach(TcInvoker* invoker, mInvoker)
        gamepad->disconnect(invoker);
}

void MasterTcInvoker::updateSensitivity(const QString &unitName, double sensitivity, const QList<bool> &inverts)
{
    //@TODO
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
        for(int i=0;i<newMethod.joysticks.size(); i++)
            newMethod.joysticks[i] = Tc::NoJoystick; // reset all joysticks
        newMethod.deadMansButton = wrap.deadMansButton;

        for(int targetId; targetId < wrap.targets.size(); targetId++)
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
                        newMethod.inverts[paramId] = wrap.inverts.value(targetId, false);
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
