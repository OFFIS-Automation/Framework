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

#include "TutorialUnitInterface.h"
#include "TutorialUnit.h"
#include "TutorialGui.h"

#include <QtPlugin>
#include <QDateTime>
#include <QSettings>
#include <QDebug>

TutorialUnitInterface::TutorialUnitInterface()
{
    hasInstance = false;
    // do nothing here
}

bool TutorialUnitInterface::needsConfigFile() const
{
	// return false if you do not need a config file
    return false;
}

QString TutorialUnitInterface::name() const
{
	return "TutorialUnit";
}

QString TutorialUnitInterface::description() const
{
	QString desc;
    desc += "This RC-Unit is a virtual robot that can be used to learn the automation.";
    desc += "<br/>"; // hml line break
    desc += "";
	return desc;
}

bool TutorialUnitInterface::createConfig(const QString& configFile, QWidget* parentForGuis)
{
	Q_UNUSED(parentForGuis); // ignore the gui and supress a warning about unused variables
	
	//write something to the config file using the QSettings class
	QSettings settings(configFile, QSettings::IniFormat);
	// write the settings
	settings.setValue("creationDate", QDateTime::currentDateTime());
	return true;
}

UserRcUnit *TutorialUnitInterface::instance(RcBase &rc, const QString & /*configFile */, const QString& /* name */)
{
    if(hasInstance)
    {
        qCritical() << "Only one instance of the tutorial RC-Unit can be created";
        return 0;
    }
    hasInstance = true;

    TutorialUnit* unit = new TutorialUnit();

    // add methods that can be called from the automation:
    // rc.addMethod(methodName, shortDesc, longDesc);
    // method name must match the real method name exactly

    rc.addMethod("acquire", "Connects to the hardware", "This command connects to the real hardware.\nIt must be called prior to any other commands.");
    rc.addMethod("release", "Disconnects from the hardware", "After this command, no other commands except acquireHardware may be called.");

    rc.addMethod("getPosition", "Returns the current position", "Returns he current robot position and rotation.");
    rc.addMethod("setPosition", "Move robot to fixed position", "Moves the robot to a absolute x/y coordinate.");
    rc.addMethod("setRotation", "Rotate robot to fixed angle", "Moves the robot to an absolute rotational angle (in degrees).");
    rc.addMethod("closeGripper", "Closes the gripper", "Closes the robot's gripper and grasps one object that is between / next to the jaws.");
    rc.addMethod("openGripper", "Opens the gripper", "Opens the robot's gripper and releases any objects.");

    rc.addMethod("resetSetup", "Resets the setup", "Places the robot and the spheres to the start positions.");
    rc.addMethod("resetSetupRandom", "Resets the setup", "Places the robot at the start position and the spheres at random positions.");
    rc.addMethod("randomizeCoordinateSystem", "Sets the coordinate system randomly", "The coordinate system of the robot will be no longer in sync with the image processing coordinate system.<br />Any reset command will reset the coordinate system.");

    // if you have telecontrol methods, add them here
    // example: connects the left joystick on the gamepad. updates are only send if Button5 is pressed
    rc.registerGamepadMethod("moveGamepad", Tc::joysticks(Tc::Gamepad::LeftJoystickX, Tc::Gamepad::LeftJoystickY, Tc::Gamepad::RightJoystickX), Tc::Gamepad::LeftShoulderUpperButton);
    rc.registerConnexionMethod("moveGamepad3d", Tc::joysticks(Tc::Connexion::JoystickX, Tc::Connexion::JoystickY, Tc::Connexion::JoystickYaw), Tc::Connexion::ImplicitActivationButton);

    rc.registerButtonMethod("openGripper", Tc::Connexion::WestButton);
    rc.registerButtonMethod("closeGripper", Tc::Connexion::EastButton);

    rc.registerButtonMethod("openGripper", Tc::Gamepad::LeftShoulderLowerButton);
    rc.registerButtonMethod("closeGripper", Tc::Gamepad::RightShoulderLowerButton);

    // if you support haptic, add your haptic method here
    rc.registerHapticMethod("moveHaptic", Tc::hapticAxis(Tc::Haptic::AxisX, Tc::Haptic::AxisY, Tc::Haptic::AxisZ), Tc::Haptic::PrimaryButton);
    rc.registerButtonMethod("alternateGripper", Tc::Haptic::SecondaryButton);

    return unit;
}

QWidget *TutorialUnitInterface::settingsWidgetForInstance(UserRcUnit *instance)
{
    return new TutorialGui(qobject_cast<TutorialUnit*>(instance));
}

void TutorialUnitInterface::deleteInstance(QObject *instance)
{
    delete instance;
    hasInstance = false;
}
