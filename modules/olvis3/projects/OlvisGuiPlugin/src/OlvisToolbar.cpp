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

#include "OlvisToolbar.h"
#include "ui_OlvisToolbar.h"

#include "OlvisSingleton.h"
#include <QFileDialog>

#include "CreatePlugin/CreatePluginDialog.h"
OlvisToolbar::OlvisToolbar(QWidget *parent) :
    QToolBar(parent),
    ui(new Ui::OlvisToolbar)
{
    ui->setupUi(this);
    connect(&OlvisSingleton::instance(), SIGNAL(executionStarted()), SLOT(onExecutionStarted()));
    connect(&OlvisSingleton::instance(), SIGNAL(executionFinished()), SLOT(onExecutionFinished()));
}

void OlvisToolbar::initMenu(QMenu *olvisMenu)
{
    if(olvisMenu->actions().size() > 0)
        olvisMenu->addSeparator();
    olvisMenu->addAction(ui->actionCreatePlugin);
    olvisMenu->addAction(ui->actionReloadPlugins);
}

OlvisToolbar::~OlvisToolbar()
{
    delete ui;
}

void OlvisToolbar::on_actionCreatePlugin_triggered()
{
    CreatePluginDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
        dialog.createPlugin();
}

void OlvisToolbar::on_actionReloadPlugins_triggered()
{
    emit reloadPlugins();
}

void OlvisToolbar::onExecutionStarted()
{
    ui->actionReloadPlugins->setEnabled(false);
}

void OlvisToolbar::onExecutionFinished()
{
    ui->actionReloadPlugins->setEnabled(true);
}
