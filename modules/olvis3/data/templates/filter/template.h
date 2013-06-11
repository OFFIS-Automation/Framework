#ifndef CUSTOMPLUGIN_H
#define CUSTOMPLUGIN_H
#include <filter/OlvisPlugin.h>


class CustomPlugin : public OlvisPlugin
{
    Q_OBJECT
    Q_INTERFACES(FilterPluginInterface)
    Q_PLUGIN_METADATA(IID FilterPluginInterface_iid)
};

#endif //CUSTOMPLUGIN_H
