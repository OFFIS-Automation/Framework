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

#include "ExportSensorTraceDialog.h"
#include "ui_ExportSensorTraceDialog.h"

#include <QFileDialog>

ExportSensorTraceDialog::ExportSensorTraceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportSensorTraceDialog)
{
    ui->setupUi(this);
    group1.addButton(ui->everyUpdate);
    group1.addButton(ui->singleUpdate);
    group2.addButton(ui->space);
    group2.addButton(ui->comma);
    group2.addButton(ui->semicolon);
    ui->space->setChecked(true);
    ui->everyUpdate->setChecked(true);
    ui->comboBox->setEnabled(false);

    // Hide help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

ExportSensorTraceDialog::~ExportSensorTraceDialog()
{
    delete ui;
}

void ExportSensorTraceDialog::setFilename(const QString &filename)
{
    ui->fileName->setText(filename);
}

void ExportSensorTraceDialog::setElements(const QStringList &elements)
{
    QString current = ui->comboBox->currentText();
    ui->comboBox->clear();
    ui->comboBox->addItems(elements);
    int index = ui->comboBox->findText(current);
    if(index > 0)
        ui->comboBox->setCurrentIndex(index);
}

void ExportSensorTraceDialog::setMarker(QStringList marker)
{
    QString currentStart = ui->startMarker->currentText();
    QString currentEnd = ui->endMarker->currentText();
    ui->startMarker->clear();
    ui->endMarker->clear();
    ui->startMarker->addItem(tr("File start"));
    ui->endMarker->addItem(tr("File end"));
    ui->startMarker->addItems(marker);
    ui->endMarker->addItems(marker);

    int index = ui->startMarker->findText(currentStart);
    if(index > 0)
        ui->startMarker->setCurrentIndex(index);
    index = ui->endMarker->findText(currentEnd);
    if(index > 0)
        ui->endMarker->setCurrentIndex(index);
}

QString ExportSensorTraceDialog::startMarker()
{
    if(ui->startMarker->currentIndex() == 0)
        return QString();
    else
        return ui->startMarker->currentText();
}

QString ExportSensorTraceDialog::endMarker()
{
    if(ui->endMarker->currentIndex() == 0)
        return QString();
    else
        return ui->endMarker->currentText();
}

QString ExportSensorTraceDialog::filename()
{
    return ui->fileName->text();
}

QString ExportSensorTraceDialog::seperator()
{
    if(ui->comma->isChecked())
        return ",";
    else if(ui->semicolon->isChecked())
        return ";";
    else
        return " ";
}

bool ExportSensorTraceDialog::startAtZero()
{
    return ui->startAtZero->isChecked();
}

QStringList ExportSensorTraceDialog::entriesForItems()
{
    QStringList items;
    if(ui->everyUpdate->isChecked())
    {
        for(int i=0; i<ui->comboBox->count(); i++)
            items << ui->comboBox->itemText(i);
    }
    else
        items << ui->comboBox->currentText();
    return items;
}

void ExportSensorTraceDialog::on_chooseFile_clicked()
{
    QString name = QFileDialog::getSaveFileName(this, tr("Select file"), filename(), tr("Text files (*.csv *.txt)"));
    if(!name.isEmpty())
        ui->fileName->setText(name);
}
