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

#include "CreateProjectDialog.h"
#include "ui_CreateProjectDialog.h"

#include <QFileDialog>

CreateProjectDialog::CreateProjectDialog(QString startDir, QWidget *parent) :
    QWizard(parent),
    ui(new Ui::CreateProjectDialog)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    ui->wizardPage1->init(ui->name, ui->destination);
    ui->destination->setText(startDir);

}

CreateProjectDialog::~CreateProjectDialog()
{
    delete ui;
}

QString CreateProjectDialog::getProjectFileName(QString ending)
{
    QString name = ui->name->text();
    QString dir = ui->destination->text();
    if(name.isEmpty() || dir.isEmpty() || ending.isEmpty())
        return QString();
    return QFileInfo(dir + "/" + name + "/" + name +"." + ending).absoluteFilePath();
}

void CreateProjectDialog::initLastPage()
{
    ui->file1Name->setText(getProjectFileName());
    ui->file2Name->setText(getProjectFileName("ogr"));
}

void CreateProjectDialog::on_selectDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), ui->destination->text());
    if(!dir.isEmpty())
    {
        ui->destination->setText(dir);
        ui->destination->setCursorPosition(dir.size());
    }
}
