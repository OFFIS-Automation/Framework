// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#ifndef PROJECTEDITOR_H
#define PROJECTEDITOR_H

#include <QDir>

#include <interfaces/GuiPluginInterface.h>
#include "HilecSingleton.h"
#include "scriptInteraction/ScriptUi.h"
#include "scriptInteraction/ScriptOutput.h"
#include "exception/ScriptExceptionDialog.h"
#include "HilecGuiToolbar.h"
#include "rcUnits/RcUnitWidget.h"
#include "rcUnits/RcUnitContainerWidget.h"
#include "rcUnits/HilecHelpWidget.h"
#include "debugVars/DebugVariables.h"
#include "debugVars/CallStackWidget.h"
#include "runConfigurations/ConfigurationWidget.h"
#include "telecontrol/TelecontrolWidget.h"
#include "ScriptErrors/ScriptErrorWidget.h"

class HilecGui : public QObject, public GuiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(GuiPluginInterface)
    Q_PLUGIN_METADATA(IID GuiPluginInterface_iid)
public:
    explicit HilecGui(QObject *parent = 0);
    virtual QString getName() const;
    virtual void setModelInterface(const QString& name, QObject* modelInterface);
    virtual bool requirementsMet();
    virtual void initialize(const QString& pluginDir);
    virtual void deinitialize();
    virtual void setGuiInterface(const QString& name, QObject* connectObject);
    virtual QObject* getConnectObject();
    virtual void addElements(MainWindowInterface* mainWindow);
    virtual void start();
    void loadProject(const QString& projectFile);
    void closeProject();

signals:
    void focusLine(const QString& file, int line);
    void compileFile(const QString& filename);

protected slots:
    void checkFile(const QString& filename);
protected:
    QStringList allPythonFiles(QDir dir, int depth = 1);
    MainWindowInterface* mMainWindow;
    QList<QDockWidget*> mRcUnitGuis;
    // add window objects here
    HilecGuiToolbar* toolbar;
    HilecInterface* hilec;
    ScriptUi* scriptUi;
    QDockWidget* scriptOutput;
    ScriptExceptionDialog* scriptException;
    RcUnitWidget* rcUnits;
    HilecHelpWidget* help;
    DebugVariables* debugVars;
    ConfigurationWidget *runConfigurations;
    TelecontrolWidget* telecontrol;
    RcUnitContainerWidget* rcContainer;
    CallStackWidget* callStack;
    ScriptErrorWidget* errors;
};

#endif // PROJECTEDITOR_H
