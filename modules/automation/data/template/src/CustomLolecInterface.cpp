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

#include "CustomLolecInterface.h"
#include "CustomLolec.h"
#include "CustomLolecGui.h"

#include <QtPlugin>
#include <QDateTime>
#include <QSettings>
#include <QDebug>

CustomLolecInterface::CustomLolecInterface()
{
  // do nothing here
}

bool CustomLolecInterface::needsConfigFile() const
{
	// return false if you do not need a config file
	return true;
}

QString CustomLolecInterface::name() const
{
	return "CustomLolec";
}
QString CustomLolecInterface::description() const
{
	QString desc;
    desc += "Auto generated description for the <strong>CustomLolec</strong> automation addon";
    desc += "<br />"; // hml line break
    desc += "<em>This should be rewritten by the user</em>";
	return desc;
}

bool CustomLolecInterface::createConfig(const QString& configFile, QWidget* parentForGuis)
{
	Q_UNUSED(parentForGuis); // ignore the gui and supress a warning about unused variables
	
	//write something to the config file using the QSettings class
	QSettings settings(configFile, QSettings::IniFormat);
	// write the settings
	settings.setValue("creationDate", QDateTime::currentDateTime());
	return true;
}

QObject* CustomLolecInterface::instance(RcBase &rc, const QString &configFile, const QString& name)
{

    //@TODO: do someting with the config file if you need it.
	QSettings settings(configFile, QSettings::IniFormat);
	QDateTime creationDate = settings.value("creationDate").toDateTime();

	qDebug() << "This instance of <CustomLolec> was created at "  << creationDate;
	
    CustomLolec* lolec = new CustomLolec();

    // add methods that can be called from the automation:
    // rc.addMethod(methodName, shortDesc, longDesc);
    // method name must match the real method name exactly

    rc.addMethod("acquireHardware", "Connects to the hardware", "This command connects to the real hardware.\nIt must be called prior to any other commands");
    rc.addMethod("releaseHardware", "Disconnects from the hardware", "After this command, no other commands except acquireHardware may be called");

    // if you have telecontrol methods, add them here
    // example: connects the left joystick on the gamepad. updates are only send if Button5 is pressed
    //rc.registerUpdateMethod("moveGamepad", Tc::joysticks(Tc::XAxisLeft, Tc::YAxisLeft), Tc::Button5);

    return lolec;
}

QWidget* CustomLolecInterface::guiForInstance(QObject *instance)
{
    return new CustomLolecGui(qobject_cast<CustomLolec*>(instance));
}

Q_EXPORT_PLUGIN2(CustomLolecInterface, CustomLolecInterface)
