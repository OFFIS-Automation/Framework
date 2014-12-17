#ifndef CUSTOMRCUNITINTERFACE_H
#define CUSTOMRCUNITINTERFACE_H
#include <RcUnitInterface.h>

class CustomRcUnitInterface : public QObject, public RcUnitInterface
{
    Q_OBJECT
    /* OFFIS RC-Unit Plugin definitions */
    Q_INTERFACES(RcUnitInterface)
    Q_PLUGIN_METADATA(IID RcUnitInterface_iid)

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
    * should return true on success. A gui dialog or wizard can be used with the given parent.
    * if something failes, the method should print (qCritical) an error (qCritical) and return false
    */
    virtual bool createConfig(const QString& configFile, QWidget* parentForGuis);

    /**
    * is called when a project with this plugin is loaded. Returns the plugin object and registers the
    * functions to the rc object. Config file is only valid when the plugin needs a config file.
    * the name contains the user-given name for this instance
    */
    UserRcUnit* instance(RcBase& rc, const QString& configFile, const QString& name);

};

#endif // CUSTOMRCUNITINTERFACE_H
