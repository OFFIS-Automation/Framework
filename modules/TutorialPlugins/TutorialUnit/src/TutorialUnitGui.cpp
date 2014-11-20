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

#include "TutorialUnitGui.h"
#include "ui_TutorialUnitGui.h"
#include "TutorialUnit.h"

TutorialUnitGui::TutorialUnitGui(TutorialUnit *control, GraphicsView *scene) :
    ui(new Ui::TutorialUnitGui),
    mController(control),
    mScene(scene)
{
    ui->setupUi(this);
    connect(mController, SIGNAL(connectStatusChanged(bool)), SLOT(updateStatus(bool)));
    connect(mController, SIGNAL(positionUpdated(QPointF,qreal)), SLOT(onPositionUpdate(QPointF, qreal)));
    connect(this, SIGNAL(connectClicked()), mController, SLOT(acquireHardware()));
    connect(this, SIGNAL(disconnectClicked()), mController, SLOT(releaseHardware()));
    connect(this, SIGNAL(resetSystem()), mController, SLOT(resetSetup()));
    connect(this, SIGNAL(resetSystemRandom()), mController, SLOT(resetSetupRandom()));
    scene->show();
    scene->hide();
}

TutorialUnitGui::~TutorialUnitGui()
{
    delete ui;
    delete mScene;
}

void TutorialUnitGui::updateStatus(bool connected)
{
    ui->connectButton->setText(connected ? "Disconnect" : "Connect");
    ui->connectButton->setChecked(connected);
}

void TutorialUnitGui::onPositionUpdate(QPointF p, qreal rot)
{
    ui->x->setValue(p.x());
    ui->y->setValue(p.y());
    ui->rot->setValue(rot);
}

void TutorialUnitGui::on_connectButton_clicked(bool checked)
{
    if(checked)
        emit connectClicked();
    else
        emit disconnectClicked();
}

void TutorialUnitGui::on_resetButton_clicked()
{
    emit resetSystem();
}

void TutorialUnitGui::on_resetRandomButton_clicked()
{
    emit resetSystemRandom();
}

