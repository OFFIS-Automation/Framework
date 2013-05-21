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

#include "PluginLoader.h"
#include <QSplashScreen>
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QPluginLoader>
#include <QSplashScreen>

#include <interfaces/GuiPluginInterface.h>
#include <interfaces/ModelPluginInterface.h>

#include "PerspectiveControl.h"

PluginLoader::PluginLoader(const QString &path)
{
    mPath = path;

}

PluginLoader::~PluginLoader()
{
    unload();
}

void PluginLoader::load(QSplashScreen *splash)
{
    QDir current = QDir::current();
    QDir pluginDir(mPath);
    qDebug() << mPath;
    QDir::setCurrent(pluginDir.canonicalPath());
    qDebug() << pluginDir.canonicalPath();

#ifdef Q_OS_LINUX
    QStringList pluginFiles = pluginDir.entryList(QStringList("*.dll") << "*.dylib" << "*.so", QDir::Files | QDir::NoDotAndDotDot);
#else
    QStringList pluginFiles = pluginDir.entryList(QStringList("*.dll") << "*.dylib" << "*.so", QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
#endif

    foreach(QString pluginFile, pluginFiles)
    {
        QPluginLoader loader(pluginDir.absoluteFilePath(pluginFile));
        loader.setLoadHints(QLibrary::ResolveAllSymbolsHint);
        QObject* obj = loader.instance();
        if(!obj) {
            qDebug() << loader.errorString();
            continue;
        }
        ModelPluginInterface* modelPlugin = qobject_cast<ModelPluginInterface*>(obj);
        GuiPluginInterface* guiPlugin = qobject_cast<GuiPluginInterface*>(obj);
        if(modelPlugin)
        {
            QString name = modelPlugin->getName();
            mModelPlugins[name] = modelPlugin;
            mLoadedPlugins.append(pluginDir.absoluteFilePath(pluginFile));
        }
        else if(guiPlugin)
        {
            QString name = guiPlugin->getName();
            qDebug() << "Found GUI plugin:" << name;
            mGuiPlugins[name] = guiPlugin;
            mLoadedPlugins.append(pluginDir.absoluteFilePath(pluginFile));
        }
        else
        {
            loader.unload();
        }
    }
    QMapIterator<QString, ModelPluginInterface*> models(mModelPlugins);
    while(models.hasNext())
    {
        ModelPluginInterface* model = models.next().value();
        splash->showMessage(QApplication::translate("SplashScreen", "Initializing plugin: %1").arg(model->getName()));
        QApplication::processEvents();

        model->initialize(mPath);
        qDebug() << "Model" << model->getName() << "success:" << model->requirementsMet();
    }
    QMapIterator<QString, GuiPluginInterface*> guis(mGuiPlugins);
    while(guis.hasNext())
    {
        GuiPluginInterface* gui = guis.next().value();
        splash->showMessage(QApplication::translate("SplashScreen", "Initializing plugin: %1").arg(gui->getName()));
        QApplication::processEvents();
        models.toFront();
        while(models.hasNext())
        {
            ModelPluginInterface* model = models.next().value();
            if(model->requirementsMet())
            {
                qDebug() << "Configuring" << gui->getName() << "with" << model->getName();
                gui->setModelInterface(model->getName(), model->getInterfaceObject());
            }
        }
        qDebug() << "Gui" << gui->getName() << "success:" << gui->requirementsMet();
        if(gui->requirementsMet())
            gui->initialize(mPath);
    }
    guis.toFront();
    while(guis.hasNext())
    {
        GuiPluginInterface* gui = guis.next().value();
        QMapIterator<QString, GuiPluginInterface*> otherGuis(mGuiPlugins);
        {
            if(!gui->requirementsMet())
                continue;
            while(otherGuis.hasNext())
            {
                GuiPluginInterface* otherGui = otherGuis.next().value();
                if(gui == otherGui)
                    continue;
                if(!otherGui->requirementsMet())
                    continue;
                gui->setGuiInterface(otherGui->getName(), otherGui->getConnectObject());
            }
        }
    }
    guis.toFront();
    while(guis.hasNext())
    {
        GuiPluginInterface* gui = guis.next().value();
        if(!gui->requirementsMet())
        {
            mGuiPlugins.remove(gui->getName());
        }
    }
    QDir::setCurrent(current.canonicalPath());
}

void PluginLoader::configure(QSplashScreen *splash, PerspectiveControl *mainWindow, bool nostart)
{
    QMapIterator<QString, GuiPluginInterface*> guis(mGuiPlugins);
    while(guis.hasNext())
    {
        GuiPluginInterface* gui = guis.next().value();
        if(gui->requirementsMet())
        {
            gui->addElements(mainWindow);
            splash->showMessage(QApplication::translate("SplashScreen", "Starting plugin: %1").arg(gui->getName()));
            QApplication::processEvents();
            if(!nostart)
                gui->start();
        }
    }
}

void PluginLoader::deinitializeGuis()
{
    foreach(GuiPluginInterface* gui, mGuiPlugins.values())
    {
        gui->deinitialize();
    }
    mGuiPlugins.clear();
}

void PluginLoader::deinitializeModels()
{
    foreach(ModelPluginInterface* model, mModelPlugins.values())
    {
        model->deinitialize();
    }
    mModelPlugins.clear();
}

void PluginLoader::unload()
{
    deinitializeGuis();
    deinitializeModels();
    foreach(QString path, mLoadedPlugins)
    {
        QPluginLoader loader(path);
        loader.unload();
    }
    mLoadedPlugins.clear();
}

void PluginLoader::openProject(const QString &projectFile)
{
    foreach(GuiPluginInterface* gui, mGuiPlugins.values())
    {
        gui->loadProject(projectFile);
    }
}

void PluginLoader::closeProject()
{
    foreach(GuiPluginInterface* gui, mGuiPlugins.values())
    {
        gui->closeProject();
    }
}
