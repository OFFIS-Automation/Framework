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


#include "TutorialGui.h"
#include "ui_TutorialGui.h"
#include "TutorialUnit.h"

TutorialGui::TutorialGui(TutorialUnit *control) :
    ui(new Ui::TutorialGui),
    mController(control)
{
    ui->setupUi(this);

    connect(this, SIGNAL(resetSystem()), mController, SLOT(resetSetup()), Qt::QueuedConnection);
    connect(this, SIGNAL(resetSystemRandom()), mController, SLOT(resetSetupRandom()), Qt::QueuedConnection);
}

TutorialGui::~TutorialGui()
{
    delete ui;
}

void TutorialGui::on_resetButton_clicked()
{
    emit resetSystem();
    parentWidget()->hide();
}

void TutorialGui::on_randomizeButton_clicked()
{
    emit resetSystemRandom();
    parentWidget()->hide();
}
