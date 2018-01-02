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

#include "DebugVariables.h"
#include "ui_DebugVariables.h"

#include "../HilecSingleton.h"

DebugVariables::DebugVariables(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DebugVariables)
{
    ui->setupUi(this);
    ui->treeView->setModel(HilecSingleton::hilec()->getDebugVars());
    connect(ui->treeView->model(), SIGNAL(lockChanged(bool)), ui->dockWidgetContents, SLOT(setDisabled(bool)));
}

DebugVariables::~DebugVariables()
{
    delete ui;
}
