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

#include "HilecGui.h"

#include <QtPlugin>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QProgressDialog>
#include <QShortcut>

HilecGui::HilecGui(QObject *parent) :
    QObject(parent)
{
    hilec = 0;
    mMainWindow = 0;
}

QString HilecGui::getName() const
{
    return "HilecGui";
}

void HilecGui::setModelInterface(const QString& name, QObject* object)
{
    if(hilec || name != "Hilec")
        return;
    hilec = static_cast<HilecInterface*>(object);
    HilecSingleton::setHilec(hilec);
}

bool HilecGui::requirementsMet()
{
    return hilec;
}

void HilecGui::initialize(const QString&)
{
    toolbar = new HilecGuiToolbar();
    scriptOutput = new ScriptOutput();
    scriptUi = new ScriptUi();
    scriptException = new ScriptExceptionDialog(scriptUi);
    rcUnits = new RcUnitWidget();
    help = new HilecHelpWidget();
    debugVars = new DebugVariables();
    callStack = new CallStackWidget();
    runConfigurations = new ConfigurationWidget();
    telecontrol = new TelecontrolWidget();
    rcContainer = new RcUnitContainerWidget();
    errors = new ScriptErrorWidget();

    connect(toolbar, SIGNAL(createGamepadMapping()), telecontrol, SLOT(editButtonAssignment()));
    connect(toolbar, SIGNAL(addRcServer()), rcUnits, SLOT(addRcServer()));
    connect(scriptException, SIGNAL(focusLine(QString, int)), SIGNAL(focusLine(QString,int)));
    connect(callStack, SIGNAL(focusLine(QString, int)), SIGNAL(focusLine(QString,int)));
    connect(errors, SIGNAL(focusLine(QString, int)), SIGNAL(focusLine(QString,int)));
    connect(hilec, SIGNAL(breakpointHit(QString,int)), SIGNAL(focusLine(QString,int)));
    hilec->connect(this, SIGNAL(compileFile(QString)), SLOT(compileFile(QString)), Qt::QueuedConnection);

    connect(rcUnits, SIGNAL(addPluginToProject()), toolbar, SLOT(on_actionAddRcUnit_triggered()));
    closeProject();
}

void HilecGui::deinitialize()
{

}

void HilecGui::setGuiInterface(const QString& name, QObject* obj)
{
    if(name == "ProjectEditor")
    {
        toolbar->connect(obj, SIGNAL(activeFileChanged(QString)),SLOT(setCurrentFile(QString)));
        connect(obj ,SIGNAL(fileSaved(QString)), SLOT(checkFile(QString)));
        connect(obj ,SIGNAL(fileOpened(QString)), SLOT(checkFile(QString)));
        connect(obj, SIGNAL(clickedProblem(QString,int)), errors, SLOT(selectProblem(QString,int)));
    }
}


void HilecGui::checkFile(const QString &filename)
{
    if(!filename.endsWith(".py"))
        return;
    emit compileFile(filename);
}



void HilecGui::loadProject(const QString &projectFile)
{
    rcUnits->setProjectFile(projectFile);
    hilec->loadFromFile(projectFile);
    runConfigurations->setConfigFile(projectFile);
    toolbar->setConfigFile(projectFile);
    callStack->setProjectFile(projectFile);
    errors->setProjectFile(projectFile);
    telecontrol->setConfigFile(projectFile);

    // compile all files
    foreach(const QString& file, allPythonFiles(QFileInfo(projectFile).absoluteDir(), 1))
    {
        emit compileFile(file);
    }

    runConfigurations->enableContent();
    rcUnits->enableContent();

    toolbar->setEnabled(true);
    scriptUi->setEnabled(true);
    scriptOutput->setEnabled(true);
    scriptException->setEnabled(true);
    rcUnits->setEnabled(true);
    help->setEnabled(true);
    debugVars->setEnabled(true);
    runConfigurations->setEnabled(true);
    telecontrol->setEnabled(true);
    rcContainer->setEnabled(true);
    callStack->setEnabled(true);
    errors->setEnabled(true);
}

void HilecGui::closeProject()
{
    hilec->quit();
    QProgressDialog dlg(tr("Waiting for automation script to stop"), tr("Continue"), 0, 0);
    connect(hilec,SIGNAL(scriptExecutionFinished()), &dlg, SLOT(close()));
    while(!hilec->waitForStop(1000))
    {
        dlg.exec();
    }

    runConfigurations->disableContent();
    rcUnits->disableContent();

    rcUnits->clear();
    errors->clear();
    telecontrol->clear();
    hilec->releaseConfig();

    toolbar->setEnabled(false);
    scriptUi->setEnabled(false);
    scriptOutput->setEnabled(false);
    scriptException->setEnabled(false);
    rcUnits->setEnabled(false);
    help->setEnabled(false);
    debugVars->setEnabled(false);
    runConfigurations->setEnabled(false);
    telecontrol->setEnabled(false);
    rcContainer->setEnabled(false);
    callStack->setEnabled(false);
    errors->setEnabled(false);
}

QObject* HilecGui::getConnectObject()
{
    return this;
}

void HilecGui::addElements(MainWindowInterface* mainWindow)
{
    mMainWindow = mainWindow;
    mainWindow->addToolBar(toolbar, tr("RC-Unit"));
    mainWindow->addToolBar(toolbar, tr("Scripting"));

    mainWindow->addDockWidget(Qt::BottomDockWidgetArea, scriptOutput, tr("Scripting"));
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, scriptUi, tr("Scripting"));
    mainWindow->addDockWidget(Qt::TopDockWidgetArea, help, tr("Scripting"));
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, debugVars, tr("Scripting"));
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, callStack, tr("Scripting"));
    mainWindow->addDockWidget(Qt::BottomDockWidgetArea, errors, tr("Scripting"));
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, runConfigurations, tr("Scripting"));

    mainWindow->addDockWidget(Qt::RightDockWidgetArea, rcUnits, tr("RC-Unit"));
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, telecontrol, tr("RC-Unit"));
    mainWindow->addDockWidget(Qt::TopDockWidgetArea, rcContainer, tr("RC-Unit"));

    toolbar->createMenu(&mainWindow->getMenu(tr("RC-Unit")));
    toolbar->addHelpToMenu(&mainWindow->getMenu(tr("Help")));

    PerspectiveInterface& perspective = mainWindow->getPerspective(tr("Automation"));
    perspective.setCentralWidget(help, 1);
    perspective.addDockWidget(scriptOutput);
    perspective.addDockWidget(errors);
    perspective.addDockWidget(scriptUi);
    perspective.addDockWidget(runConfigurations);
    perspective.addToolbar(toolbar);

    PerspectiveInterface& unitPerspective = mainWindow->getPerspective(tr("RC-Units"));
    unitPerspective.setCentralWidget(help, 1);
    unitPerspective.addDockWidget(rcContainer, 1);
    unitPerspective.addDockWidget(telecontrol);
    unitPerspective.addDockWidget(rcUnits);

    PerspectiveInterface& debugPerspective = mainWindow->getPerspective(tr("Debug"));
    debugPerspective.setCentralWidget(help, 1);
    debugPerspective.addDockWidget(scriptOutput);
    debugPerspective.addDockWidget(errors);
    debugPerspective.addDockWidget(debugVars);
    debugPerspective.addDockWidget(callStack);
    debugPerspective.addDockWidget(scriptUi);
    debugPerspective.addDockWidget(runConfigurations);
    debugPerspective.addToolbar(toolbar);
}

void HilecGui::start()
{
}

QStringList HilecGui::allPythonFiles(QDir dir, int depth)
{
    QStringList results;
    QList<QFileInfo> entries = dir.entryInfoList(QStringList("*.py"), QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach(QFileInfo info, entries)
    {
        if(info.isDir())
        {
            if(depth>0)
                results << allPythonFiles(QDir(info.absoluteFilePath()), depth - 1);
        }
        else
            results << info.absoluteFilePath();
    }
    return results;
}
