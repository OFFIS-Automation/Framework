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

#ifndef GUIPLUGININTERFACE_H
#define GUIPLUGININTERFACE_H

#include <QMainWindow>
#include <QMenu>
#include "logging.h"

class PerspectiveInterface
{
public:
    virtual void addToolbar(QToolBar* bar) = 0;
    virtual void setCentralWidget(QDockWidget* w, int screen = 0) = 0;
    virtual void addDockWidget(QDockWidget* w, int screen = 0) = 0;
};

class MainWindowInterface
{
public:
    virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget, const QString& group) = 0;
    virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget, Qt::Orientation orientation, const QString& group) = 0;
    virtual void removeDockWidget(QDockWidget * dockwidget) = 0;
    virtual void addToolBar(Qt::ToolBarArea area, QToolBar * toolbar, const QString& group) = 0;
    virtual void addToolBar(QToolBar * toolbar, const QString& group) = 0;
    inline QMenu& getMenu(QString name, QString subMenu) { return getMenu(name, QStringList(subMenu)); }
    virtual QMenu& getMenu(QString name, QStringList subMenu = QStringList()) = 0;
    virtual PerspectiveInterface& getPerspective(const QString& name) = 0;
};

class GuiPluginInterface
{
public:
    virtual QString getName() const = 0;
    virtual void setModelInterface(const QString& name, QObject* modelInterface) = 0;
    virtual bool requirementsMet() = 0;
    virtual void initialize(const QString& pluginDir) = 0;
    virtual void deinitialize() = 0;
    virtual void setGuiInterface(const QString& name, QObject* connectObject) = 0;
    virtual QObject* getConnectObject() = 0;
    virtual void addElements(MainWindowInterface* mainWindow) = 0;
    virtual void start() {}
    virtual void loadProject(const QString& projectFile) { Q_UNUSED(projectFile); }
    virtual void closeProject() {}
};
#define GuiPluginInterface_iid "com.amir.toolkit.GuiInterface/1.1"
Q_DECLARE_INTERFACE(GuiPluginInterface, GuiPluginInterface_iid)

#endif // GUIPLUGININTERFACE_H
