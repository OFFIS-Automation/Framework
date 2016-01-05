// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QString>
#include <QMap>
#include <QObject>

class PerspectiveControl;

class ModelPluginInterface;
class GuiPluginInterface;
class QSplashScreen;

class PluginLoader : public QObject
{
    Q_OBJECT
public:
    PluginLoader(const QString& path);
    virtual ~PluginLoader();
    void load(QSplashScreen* splash);
    void configure(QSplashScreen *splash, PerspectiveControl* mainWindow, bool nostart);
    void deinitializeGuis();
    void deinitializeModels();
    void unload();
public slots:
    void openProject(const QString& projectFile);
    void closeProject();
protected:
    QString mPath;
    QMap<QString, ModelPluginInterface*> mModelPlugins;
    QMap<QString, GuiPluginInterface*> mGuiPlugins;
    QList<QString> mLoadedPlugins;
};

#endif // PLUGINLOADER_H
