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

#include "ScriptStringInput.h"
#include "ui_ScriptStringInput.h"
#include <QFileDialog>

ScriptStringInput::ScriptStringInput(const QString &data, const QVariantMap constrains) :
    ui(new Ui::ScriptStringInput)
{
    ui->setupUi(this);
    ui->lineEdit->setText(data);
    mMode = constrains.value("fileMode", -1).toInt();
    if(mMode < 0)
        ui->selectButton->hide();
}

ScriptStringInput::~ScriptStringInput()
{
    delete ui;
}

QVariant ScriptStringInput::getValue()
{
    return ui->lineEdit->text();
}

void ScriptStringInput::on_selectButton_clicked()
{
    QString data;
    if(mMode == QFileDialog::AnyFile)
        data = QFileDialog::getSaveFileName(this, tr("Select output file"), ui->lineEdit->text());
    else if(mMode == QFileDialog::ExistingFile)
        data = QFileDialog::getOpenFileName(this, tr("Select input file"), ui->lineEdit->text());
    else if(mMode == QFileDialog::Directory)
        data = QFileDialog::getExistingDirectory(this, tr("Select directory"), ui->lineEdit->text());
    if(!data.isEmpty())
        ui->lineEdit->setText(data.replace("\\", "/"));
}
