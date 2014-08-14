#ifndef CUSTOMRCUNITINTERFACE_H
#define CUSTOMRCUNITINTERFACE_H

#include "RcUnitInterface.h"

class CustomRcUnitInterface : public QObject, public RcUnitInterface
{
    Q_OBJECT
    Q_INTERFACES(RcUnitInterface)
public:
    CustomRcUnitInterface();
	
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
    QObject* instance(RcBase& rc, const QString& configFile, const QString& name);
	
	/**
	* should return a small widget with status information about the rcUnit instance. 
	* the method is called after instance is called and the instance object is exactly the same
	* if the plugin has no gui, this method may return 0.
	*/
    QWidget* guiForInstance(QObject *instance);
};

#endif // CUSTOMRCUNITINTERFACE_H
