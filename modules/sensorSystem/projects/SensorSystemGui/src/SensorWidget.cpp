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

#include "SensorWidget.h"
#include "ui_SensorWidget.h"

#include "AddSensorTraceDialog.h"
#include "SensorWidgetLine.h"
#include "ExportSensorTraceDialog.h"
#include "SensorTraceExport.h"

#include <QFileDialog>
#include <QSettings>

SensorWidget::SensorWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SensorWidget),
    mWriter(SensorDataWriter::instance())
{
    ui->setupUi(this);
    ui->exportTrace->setEnabled(false);

    connect(&mWriter, SIGNAL(itemTraceAdded(QString)), SLOT(traceItemAdded(QString)));
    connect(&mWriter, SIGNAL(itemTraceRemoved(QString)), SLOT(traceItemRemoved(QString)));

    QStringList items = QSettings().value("sensorSystemGui/selectedTraces").toStringList();
    foreach(QString item, items)
        mWriter.add(item);
}

SensorWidget::~SensorWidget()
{
    delete ui;
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
    bool hasSelection = false;
    foreach(SensorWidgetLine* line, mElements)
    {
        if(line->isSelected())
        {
            hasSelection = true;
            break;
        }
    }
    ui->exportTrace->setEnabled(hasSelection);

}




void SensorWidget::on_exportTrace_clicked()
{
    QStringList elements;
    foreach(SensorWidgetLine* line, mElements)
    {
        if(line->isSelected())
            elements << line->name();
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
    QSettings settings;
    settings.setValue("sensorSystemGui/selectedTraces", QStringList(mElements.keys()));
}

void SensorWidget::on_addMarker_clicked()
{
    QString text = ui->markerValue->currentText();
    mWriter.mark(text);
}
