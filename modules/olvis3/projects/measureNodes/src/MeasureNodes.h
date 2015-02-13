#ifndef MEASURENODES_H
#define MEASURENODES_H
#include <filter/OlvisPlugin.h>


class MeasureNodes : public OlvisPlugin
{
    Q_OBJECT
    Q_INTERFACES(FilterPluginInterface)
    Q_PLUGIN_METADATA(IID FilterPluginInterface_iid)
};

#endif //MEASURENODES_H
