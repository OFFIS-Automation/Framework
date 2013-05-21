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

#include <filter/OlvisPlugin.h>
#include <QtPlugin>

#include <filter/PluginInterface.h>
#include <core/SrcFilter.h>


OlvisPlugin::OlvisPlugin()
{
    QList<UserFilterFactoryI*> factories(PluginInterface::instance().factories);
    foreach(UserFilterFactoryI* factory, factories)
    {
        factory->setup();
    }
}

int OlvisPlugin::numFilters()
{
    int numFilters = PluginInterface::instance().creators.size();
    QList<UserFilterFactoryI*> factories(PluginInterface::instance().factories);
    foreach(UserFilterFactoryI* factory, factories)
    {
        numFilters += factory->numFilters();
    }
    return numFilters;
}

SrcFilter *OlvisPlugin::createFilter(int num)
{
    int simpleFilters = PluginInterface::instance().creators.size();
    if(num < simpleFilters)
        return new SrcFilter(PluginInterface::instance().creators[num]->instance());
    num -= simpleFilters;
    QList<UserFilterFactoryI*> factories(PluginInterface::instance().factories);
    foreach(UserFilterFactoryI* factory, factories)
    {
        if(num < factory->numFilters())
            return new SrcFilter(factory->instance(num));
        num -= factory->numFilters();
    }
    return 0;
}

