#ifndef PYLONPLUGIN_H
#define PYLONPLUGIN_H
#include <filter/OlvisPlugin.h>


class PylonPlugin : public OlvisPlugin
{
    Q_OBJECT
    Q_INTERFACES(FilterPluginInterface)
    Q_PLUGIN_METADATA(IID FilterPluginInterface_iid)
};

#endif //PYLONPLUGIN_H
