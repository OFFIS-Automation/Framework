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

#include "PluginContainer.h"

PluginContainer::PluginContainer()
{

}
PluginContainer& PluginContainer::getInstance()
{
    static PluginContainer inst;
    return inst;
}

void PluginContainer::addPlugin(OlvisGuiPluginInterface *plugin)
{
    mPlugins.append(plugin);
}

PortEditInterface* PluginContainer::portEditFor(const PortInfo& info)
{
    QListIterator<OlvisGuiPluginInterface*> plugins(mPlugins);
    while(plugins.hasNext())
    {
        OlvisGuiPluginInterface* plugin = plugins.next();
        PortEditInterface* edit = plugin->portEditFor(info);
        if(edit)
            return edit;
    }
    return 0;
}
