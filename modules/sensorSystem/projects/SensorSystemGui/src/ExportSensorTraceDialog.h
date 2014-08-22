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

#ifndef EXPORTSENSORTRACEDIALOG_H
#define EXPORTSENSORTRACEDIALOG_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class ExportSensorTraceDialog;
}

class ExportSensorTraceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExportSensorTraceDialog(QWidget *parent = 0);
    ~ExportSensorTraceDialog();
    void setFilename(const QString& filename);
    void setElements(const QStringList& elements);
    void setMarker(QStringList marker);
    QString startMarker();
    QString endMarker();
    QString filename();
    QString seperator();
    bool startAtZero();
    bool addHeaders();
    QStringList entriesForItems();
private slots:
    void on_chooseFile_clicked();

private:
    Ui::ExportSensorTraceDialog *ui;
    QButtonGroup group1, group2;
};

#endif // EXPORTSENSORTRACEDIALOG_H
