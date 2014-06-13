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

#include "ShowAssignmentButton.h"
#include "ui_ShowAssignmentButton.h"

ShowAssignmentButton::ShowAssignmentButton(QString unit, bool hideEdit) :
    QWidget(),
    ui(new Ui::ShowAssignmentButton)
{
    mUnit = unit;
    ui->setupUi(this);
    if(hideEdit)
        ui->edit->setEnabled(false);
}

ShowAssignmentButton::~ShowAssignmentButton()
{
    delete ui;
}

void ShowAssignmentButton::on_show_clicked()
{
    emit openButtonAssignment(mUnit);
}

void ShowAssignmentButton::on_edit_clicked()
{
    emit editButtonAssignment(mUnit);
}
