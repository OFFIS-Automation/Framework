// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#include <QDir>
#include <QPluginLoader>

PluginContainer::PluginContainer()
{

}
PluginContainer& PluginContainer::getInstance()
{
    static PluginContainer inst;
    return inst;
}

void PluginContainer::loadPlugins(QString folder)
{
    QDir searchDir(folder);
#ifdef Q_OS_LINUX
    QStringList files = searchDir.entryList(QStringList("*.dll") << "*.so" << "*.dylib", QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
#else
    QStringList files = searchDir.entryList(QStringList("*.dll") << "*.so" << "*.dylib", QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name | QDir::IgnoreCase);
#endif

    foreach(QString file, files)
    {
        QString fullPath = searchDir.absoluteFilePath(file);
        QPluginLoader loader(fullPath);
        QObject* obj = loader.instance();
        if(obj)
        {
            OlvisGuiPluginInterface* plugin = qobject_cast<OlvisGuiPluginInterface*>(obj);
            if(plugin)
               addPlugin(plugin);
        }
    }
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

OverlayInterface *PluginContainer::overlayFor(const QString &name, bool output, bool isMasterOverlay, OlvisInterface *visionInterface)
{
    foreach(OlvisGuiPluginInterface* plugin, mPlugins)
    {
        OverlayInterface* overlay = plugin->overlayFor(name, output, isMasterOverlay, visionInterface);
        if(overlay)
            return overlay;
    }
    return 0;
}

OverlayInterface *PluginContainer::overlayFor(const PortInfo &info, bool isOutput, bool isMasterOverlay, OlvisInterface *visionInterface)
{
    foreach(OlvisGuiPluginInterface* plugin, mPlugins)
    {
        OverlayInterface* overlay = plugin->overlayFor(info, isOutput, isMasterOverlay, visionInterface);
        if(overlay)
            return overlay;
    }
    return 0;
}
