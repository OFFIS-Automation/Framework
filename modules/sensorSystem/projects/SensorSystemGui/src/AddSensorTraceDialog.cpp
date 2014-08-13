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

#include "AddSensorTraceDialog.h"
#include "ui_AddSensorTraceDialog.h"

#include <SensorDataSystem.h>

AddSensorTraceDialog::AddSensorTraceDialog(const QStringList& blockedItems, QWidget *parent) :
    QDialog(parent),
    mBlockedItems(blockedItems.toSet()),
    ui(new Ui::AddSensorTraceDialog)
{
    ui->setupUi(this);
    on_showInactive_toggled(ui->showInactive->isChecked());

    // Hide help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AddSensorTraceDialog::~AddSensorTraceDialog()
{
    delete ui;
}

QString AddSensorTraceDialog::selection()
{
    return ui->comboBox->currentText();
}

void AddSensorTraceDialog::on_showInactive_toggled(bool checked)
{
    QSet<QString> set;
    if(checked)
        set = SensorSystemInterface::allConsumers();
    else
        set = SensorSystemInterface::activeConsumers();

    set -= mBlockedItems;
    QStringList list(set.toList());
    list.sort();
    QString current = ui->comboBox->currentText();
    ui->comboBox->clear();
    ui->comboBox->addItems(list);
    int index = ui->comboBox->findText(current);
    if(index >= 0)
        ui->comboBox->setCurrentIndex(index);
}
