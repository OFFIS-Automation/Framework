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

#include "SensorWidget.h"
#include "ui_SensorWidget.h"

#include "AddSensorTraceDialog.h"
#include "SensorWidgetLine.h"
#include "ExportSensorTraceDialog.h"
#include "SensorTraceExport.h"

#include <QFileDialog>
#include <QSettings>
#include <QDebug>

SensorWidget::SensorWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SensorWidget),
    mWriter(SensorDataWriter::instance())
{
    ui->setupUi(this);
    ui->exportTrace->setEnabled(false);

    connect(&mWriter, SIGNAL(itemTraceAdded(QString)), SLOT(traceItemAdded(QString)));
    connect(&mWriter, SIGNAL(itemTraceRemoved(QString)), SLOT(traceItemRemoved(QString)));
    setEnabled(false);
}

SensorWidget::~SensorWidget()
{
    delete ui;
}

void SensorWidget::load(const QString &projectFile)
{
    mProjectFile = projectFile;
    mSortedElements = QSettings(projectFile, QSettings::IniFormat).value("sensorSystemGui/selectedTraces").toStringList();
    foreach(QString item, mSortedElements)
        mWriter.add(item);
   setEnabled(true);
}

void SensorWidget::clear()
{
    setEnabled(false);
    mWriter.clear();
}

void SensorWidget::on_toolButton_clicked()
{
    AddSensorTraceDialog dialog(mElements.keys());
    if(dialog.exec() == QDialog::Accepted)
    {
        QString item = dialog.selection();
        if(item.isEmpty())
            return;
        mWriter.add(item);
    }
}

void SensorWidget::traceItemAdded(const QString& item)
{
    SensorWidgetLine* line = new SensorWidgetLine(item);
    connect(line, SIGNAL(removeTraceItem(QString)), &mWriter, SLOT(remove(QString)));
    connect(line, SIGNAL(selectionChanged()), SLOT(onSelectionChanged()));
    connect(line, SIGNAL(sortWidgets(QString,QString,bool)), SLOT(sortWidgets(QString,QString,bool)));
    mElements[item] = line;
    ui->elementLayout->addWidget(line);
    storeSelection();
}

void SensorWidget::traceItemRemoved(const QString &name)
{
    if(mElements.contains(name))
    {
        SensorWidgetLine* line = mElements.take(name);
        ui->elementLayout->removeWidget(line);
        delete line;
        onSelectionChanged();
        storeSelection();
    }
}

void SensorWidget::onSelectionChanged()
{
    Qt::CheckState state = checkSelection();
    ui->exportTrace->setEnabled(state != Qt::Unchecked);
    if(state == Qt::Checked)
        ui->allNoneBox->setCheckState(Qt::Checked);
    else if(state == Qt::Unchecked)
        ui->allNoneBox->setCheckState(Qt::Unchecked);
    else
        ui->allNoneBox->setCheckState(Qt::PartiallyChecked);
}

Qt::CheckState SensorWidget::checkSelection()
{
    bool allSelected = true;
    bool noneSelected = true;
    foreach(SensorWidgetLine* line, mElements)
    {
        if(line->isSelected())
            noneSelected = false;
        else
            allSelected = false;
    }
    if(allSelected)
        return Qt::Checked;
    else if (noneSelected)
        return Qt::Unchecked;
    else
        return Qt::PartiallyChecked;

}




void SensorWidget::on_exportTrace_clicked()
{
    QStringList elements;
    foreach(QString name, mSortedElements)
    {
        SensorWidgetLine* line = mElements.value(name, 0);
        if(line && line->isSelected())
            elements << name;
    }
    mExportDialog.setElements(elements);
    QString file = QSettings().value("sensorSystemGui/lastTraceFile").toString();
    mExportDialog.setFilename(file);
    mExportDialog.setMarker(mWriter.marker());
    if(mExportDialog.exec() != QDialog::Accepted)
        return;
    QString filename = mExportDialog.filename();
    if(filename.isEmpty())
        return;
    QSettings().setValue("sensorSystemGui/lastTraceFile", filename);
    SensorTraceExport exporter;
    exporter.setSeperator(mExportDialog.seperator());
    exporter.setStartAtZero(mExportDialog.startAtZero());
    exporter.enableHeader(mExportDialog.addHeaders());
    QStringList entries = mExportDialog.entriesForItems();
    foreach(QString element, elements)
    {
        exporter.addElement(element, entries.contains(element));
    }
    exporter.setMarkerRange(mExportDialog.startMarker(), mExportDialog.endMarker());
    exporter.exportTrace(filename);
}

void SensorWidget::storeSelection()
{
    if(!isEnabled())
        return;
    QSettings settings(mProjectFile, QSettings::IniFormat);
    QVector<QString> items;
    items.resize(mElements.size());
    foreach(QString name, mElements.keys())
    {
        int index = ui->elementLayout->indexOf(mElements.value(name));
        if(index >= 0 && index < items.size())
        {
            items[index] = name;
        }
    }
    mSortedElements = items.toList();
    settings.setValue("sensorSystemGui/selectedTraces", QStringList(items.toList()));
}

void SensorWidget::on_addMarker_clicked()
{
    QString text = ui->markerValue->currentText();
    mWriter.mark(text);
}

void SensorWidget::on_allNoneBox_clicked()
{
    Qt::CheckState state = ui->allNoneBox->checkState();
    if(state == Qt::PartiallyChecked && checkSelection() == Qt::Unchecked)
    {
        ui->allNoneBox->setCheckState(Qt::Checked);
        state = Qt::Checked;
    }
    foreach(SensorWidgetLine* line, mElements)
    {
        line->setSelected(state == Qt::Checked);
    }
}

void SensorWidget::sortWidgets(const QString &stationary, const QString &floating, bool positionAbove)
{
    SensorWidgetLine* w1 = mElements.value(stationary);
    SensorWidgetLine* w2 = mElements.value(floating);
    if(!w1 || !w2)
        return;
    ui->elementLayout->removeWidget(w2);
    int pos = ui->elementLayout->indexOf(w1);
    if(pos >= 0)
    {
        if(!positionAbove)
            pos++;
        ui->elementLayout->insertWidget(pos, w2);
    }
    storeSelection();
}


