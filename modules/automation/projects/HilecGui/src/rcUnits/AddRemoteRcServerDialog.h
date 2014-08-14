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


#ifndef ADDREMOTERCSERVERDIALOG_H
#define ADDREMOTERCSERVERDIALOG_H

#include <QDialog>

namespace Ui {
class AddRemoteRcServerDialog;
}

class AddRemoteRcServerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddRemoteRcServerDialog(const QString& projectFile, QWidget *parent = 0);
    ~AddRemoteRcServerDialog();
    void addToSettings();
private slots:
    void on_address_textChanged(const QString &arg1);
    void on_name_textChanged(const QString &arg1);
    void checkData();

    void on_port_valueChanged(int);

private:
    Ui::AddRemoteRcServerDialog *ui;
    QString mProjectFile;
};

#endif // ADDREMOTERCSERVERDIALOG_H
