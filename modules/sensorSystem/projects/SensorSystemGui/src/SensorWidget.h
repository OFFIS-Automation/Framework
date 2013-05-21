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

#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QDockWidget>
#include "SensorDataWriter.h"

#include "ExportSensorTraceDialog.h"
namespace Ui {
class SensorWidget;
}

class SensorWidgetLine;

class SensorWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit SensorWidget(QWidget *parent = 0);
    ~SensorWidget();

public slots:

private slots:
    void on_toolButton_clicked();

    void traceItemAdded(const QString& name);
    void traceItemRemoved(const QString& name);
    void onSelectionChanged();
    void on_exportTrace_clicked();
    void on_addMarker_clicked();

protected:
    void storeSelection();
private:
    ExportSensorTraceDialog mExportDialog;
    Ui::SensorWidget *ui;
    SensorDataWriter& mWriter;
    QMap<QString, SensorWidgetLine*> mElements;

};

#endif // SENSORWIDGET_H
