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

#ifndef PLUGINCONTAINER_H
#define PLUGINCONTAINER_H

#include <gui/OlvisGuiPluginInterface.h>
#include <gui/PortEditInterface.h>
#include <gui/OverlayInterface.h>

class PluginContainer
{
public:
    static PluginContainer& getInstance();
    void loadPlugins(QString folder);
    PortEditInterface* portEditFor(const PortInfo& info);
    OverlayInterface* overlayFor(const QString &name, bool output, bool isMasterOverlay, OlvisInterface *visionInterface);
    OverlayInterface* overlayFor(const PortInfo &info, bool isOutput, bool isMasterOverlay, OlvisInterface *visionInterface);
private:
    void addPlugin(OlvisGuiPluginInterface* plugin);
    PluginContainer();
    Q_DISABLE_COPY(PluginContainer)
    QList<OlvisGuiPluginInterface*> mPlugins;
};

#endif // PLUGINCONTAINER_H
