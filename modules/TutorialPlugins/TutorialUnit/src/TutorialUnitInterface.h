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

#ifndef TUTORIALUNITINTERFACE_H
#define TUTORIALUNITINTERFACE_H

#include <rc/RcUnitInterface.h>

class TutorialUnitInterface : public QObject, public RcUnitInterface
{
    Q_OBJECT
    Q_INTERFACES(RcUnitInterface)
    Q_PLUGIN_METADATA(IID RcUnitInterface_iid)

public:
    TutorialUnitInterface();

	/**
	* return true if this automation plugin needs a config file
	* this function is called when the plugin is added to a project
	*/
    virtual bool needsConfigFile() const;
	
	/**
	* returns a user friendly name of the plugin
	*/
	virtual QString name() const;
	
	/**
	* returns a user friendly description about the plugin. html can be used
	*/
    virtual QString description() const;
	
	/**
	* function is called when the plugin is added to a project and a config file is needed
	* should return true on success. If a gui dialog or wizard can be used with the given parent
	* if sometinh failes, the method should print an error and return false
	*/
	virtual bool createConfig(const QString& configFile, QWidget* parentForGuis);
	
	/**
	* is called when a project with this plugin is loaded. Returns the plugin object and registers the
	* functions to the rc object. Config file is only valid when the plugin needs a config file.
	* the name contains the user-given name given for this instance
	*/
    UserRcUnit* instance(RcBase& rc, const QString& configFile, const QString& name);
	
	/**
    * should return a small widget with settings information about the rcUnit instance.
	* the method is called after instance is called and the instance object is exactly the same
	* if the plugin has no gui, this method may return 0.
	*/
    QWidget* settingsWidgetForInstance(UserRcUnit* instance);

    void deleteInstance(QObject *instance);
protected:
    bool hasInstance;

};

#endif // TUTORIALUNITINTERFACE_H
