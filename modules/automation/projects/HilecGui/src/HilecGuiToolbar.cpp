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

#include "HilecGuiToolbar.h"
#include "ui_HilecGuiToolbar.h"

#include "HilecSingleton.h"

#include <QFileDialog>
#include "createRcUnit/CreateRcUnitDialog.h"
#include "createRcUnit/AddRcUnitWizard.h"

HilecGuiToolbar::HilecGuiToolbar(QWidget *parent) :
    QToolBar(parent),
    ui(new Ui::HilecGuiToolbar)
{
    ui->setupUi(this);
    ui->actionAutomationRunning->setVisible(false);

    connect(ui->actionQuit, SIGNAL(triggered()), HilecSingleton::hilec(), SLOT(quit()), Qt::QueuedConnection);
    connect(ui->actionPause, SIGNAL(triggered()), HilecSingleton::hilec(), SLOT(pause()), Qt::QueuedConnection);
    connect(ui->actionStepInto, SIGNAL(triggered()), HilecSingleton::hilec(), SLOT(stepInto()), Qt::QueuedConnection);
    connect(ui->actionStepOver, SIGNAL(triggered()), HilecSingleton::hilec(), SLOT(stepOver()), Qt::QueuedConnection);
    connect(ui->actionStepOut, SIGNAL(triggered()), HilecSingleton::hilec(), SLOT(stepReturn()), Qt::QueuedConnection);
    connect(this, SIGNAL(runFile(QString)), HilecSingleton::hilec(), SLOT(runFile(QString)), Qt::QueuedConnection);
    connect(HilecSingleton::hilec(), SIGNAL(scriptExecutionStarted()), SLOT(onScriptExecutionStarted()));
    connect(HilecSingleton::hilec(), SIGNAL(scriptExecutionFinished()), SLOT(onScriptExecutionFinished()));

}

HilecGuiToolbar::~HilecGuiToolbar()
{
    delete ui;
}


void HilecGuiToolbar::on_actionResume_triggered()
{
    emit runFile(mFile);
}

void HilecGuiToolbar::createMenu(QMenu *menu)
{
    if(menu->actions().size() > 0)
    {
        menu->addSeparator();
    }
    menu->addAction(ui->actionCreateRcUnit);
    menu->addAction(ui->actionAddRcUnit);
    menu->addAction(ui->actionAddRcServer);
    menu->addAction(ui->actionCreateGamepadMapping);
}

void HilecGuiToolbar::addHelpToMenu(QMenu *menu)
{
    if(menu->actions().size() > 0)
    {
        menu->insertAction(menu->actions().first(), ui->actionLolecHelp);
        menu->addSeparator();
    } else {
        menu->addAction(ui->actionLolecHelp);
    }
}

void HilecGuiToolbar::on_actionCreateRcUnit_triggered()
{
    CreateRcUnitDialog dialog(parentWidget());
    if(dialog.exec())
    {
        dialog.createPlugin();
    }
}

void HilecGuiToolbar::on_actionAddRcUnit_triggered()
{
    AddRcUnitWizard dialog(mConfigFile, parentWidget());
    if(dialog.exec())
    {
        dialog.addPlugin();
    }
}

void HilecGuiToolbar::onScriptExecutionStarted()
{
    mRunning = true;
    ui->actionAutomationIdle->setVisible(false);
    ui->actionAutomationRunning->setVisible(true);
}

void HilecGuiToolbar::onScriptExecutionFinished()
{
    mRunning = false;
    ui->actionAutomationIdle->setVisible(true);
    ui->actionAutomationRunning->setVisible(false);
}

void HilecGuiToolbar::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);
    ui->actionAddRcUnit->setEnabled(enabled);
    ui->actionAddRcServer->setEnabled(enabled);
    ui->actionCreateGamepadMapping->setEnabled(enabled);
}

void HilecGuiToolbar::on_actionCreateGamepadMapping_triggered()
{
    emit createGamepadMapping();
}

void HilecGuiToolbar::on_actionAddRcServer_triggered()
{
    emit addRcServer();
}
