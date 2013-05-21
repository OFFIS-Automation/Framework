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

#ifndef SENSORWIDGETLINE_H
#define SENSORWIDGETLINE_H

#include <QWidget>

namespace Ui {
class SensorWidgetLine;
}

class SensorWidgetLine : public QWidget
{
    Q_OBJECT
    
public:
    explicit SensorWidgetLine(const QString& title);
    ~SensorWidgetLine();
    QString name();
    bool isSelected();
signals:
    void removeTraceItem(const QString& name);
    void selectionChanged();
private slots:
    void on_toolButton_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::SensorWidgetLine *ui;
};

#endif // SENSORWIDGETLINE_H
