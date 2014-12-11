#include "CustomRcUnitInterface.h"
#include "CustomRcUnit.h"
#include "CustomRcUnitGui.h"

#include <QtPlugin>
#include <QDateTime>
#include <QSettings>
#include <QDebug>

CustomRcUnitInterface::CustomRcUnitInterface()
{
  // do nothing here
}

bool CustomRcUnitInterface::needsConfigFile() const
{
	// return false if you do not need a config file
	return true;
}

QString CustomRcUnitInterface::name() const
{
	return "CustomRcUnit";
}
QString CustomRcUnitInterface::description() const
{
	QString desc;
    desc += "Auto generated description for the <strong>CustomRcUnit</strong> automation addon";
    desc += "<br />"; // hml line break
    desc += "<em>This should be rewritten by the user</em>";
	return desc;
}

bool CustomRcUnitInterface::createConfig(const QString& configFile, QWidget* parentForGuis)
{
	Q_UNUSED(parentForGuis); // ignore the gui and supress a warning about unused variables
	
	//write something to the config file using the QSettings class
	QSettings settings(configFile, QSettings::IniFormat);
	// write the settings
	settings.setValue("creationDate", QDateTime::currentDateTime());
	return true;
}

QObject* CustomRcUnitInterface::instance(RcBase &rc, const QString &configFile, const QString& name)
{

    //@TODO: do someting with the config file if you need it.
	QSettings settings(configFile, QSettings::IniFormat);
	QDateTime creationDate = settings.value("creationDate").toDateTime();

	qDebug() << "This instance of <CustomRcUnit> was created at "  << creationDate;
	
    CustomRcUnit* rcUnit = new CustomRcUnit();

    // add methods that can be called from the automation:
    // rc.addMethod(methodName, shortDesc, longDesc);
    // method name must match the real method name exactly

    rc.addMethod("acquireHardware", "Connects to the hardware", "This command connects to the real hardware.\nIt must be called prior to any other commands");
    rc.addMethod("releaseHardware", "Disconnects from the hardware", "After this command, no other commands except acquireHardware may be called");

    // if you have telecontrol methods, add them here
    // example: connects the left joystick on the gamepad. updates are only send if Button5 is pressed
    //rc.registerGamepadMethod("moveGamepad", Tc::joysticks(Tc::LeftJoystickX, Tc::LeftJoystickY), Tc::LeftShoulderUpperButton);

    return rcUnit;
}

QWidget* CustomRcUnitInterface::guiForInstance(QObject *instance)
{
    return new CustomRcUnitGui(qobject_cast<CustomRcUnit*>(instance));
}

Q_EXPORT_PLUGIN2(CustomRcUnitInterface, CustomRcUnitInterface)
