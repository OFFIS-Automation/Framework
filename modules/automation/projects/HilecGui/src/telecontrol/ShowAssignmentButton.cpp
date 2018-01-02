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

#include "ShowAssignmentButton.h"
#include "ui_ShowAssignmentButton.h"
#include "../HilecSingleton.h"
#include <core/RcUnitHelp.h>

ShowAssignmentButton::ShowAssignmentButton(QString unitName) :
    QWidget(),
    mUnitName(unitName),
    ui(new Ui::ShowAssignmentButton)
{
    ui->setupUi(this);

    TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(mUnitName);
    QStringList gamepadDevices = HilecSingleton::hilec()->getGamepadDevices().keys();
    ui->show->setEnabled(gamepadDevices.contains(help.tcDeviceName));
}

ShowAssignmentButton::~ShowAssignmentButton()
{
    delete ui;
}

void ShowAssignmentButton::updateTelecontrolAssignment(const QString &deviceName, const QString &unitName)
{
    if(unitName == mUnitName){
        QStringList gamepadDevices = HilecSingleton::hilec()->getGamepadDevices().keys();
        ui->show->setEnabled(gamepadDevices.contains(deviceName));
    }
}

void ShowAssignmentButton::on_show_clicked()
{
    emit openButtonAssignment(mUnitName);
}

void ShowAssignmentButton::on_edit_clicked()
{
    emit editButtonAssignment(mUnitName);
}
