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

#ifndef CREATELOLECDIALOG_H
#define CREATELOLECDIALOG_H

#include <QDialog>

namespace Ui {
    class CreateLolecDialog;
}

class CreateLolecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateLolecDialog(QWidget *parent = 0);
    ~CreateLolecDialog();
    void createPlugin();

private slots:
    void on_chooseDestination_clicked();
    void on_destination_textChanged(const QString &arg1);

    void on_pluginName_textChanged(const QString &arg1);

    void check();

protected:
    QString readFile(const QString& filename);
    void writeFile(const QString& filename, const QString& data);
private:
    Ui::CreateLolecDialog *ui;
    QString mSourceDir;
};

#endif // CREATELOLECDIALOG_H
