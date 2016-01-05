// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef CREATEPLUGINDIALOG_H
#define CREATEPLUGINDIALOG_H

#include <QDialog>

namespace Ui {
    class CreatePluginDialog;
}

class CreatePluginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePluginDialog(QWidget *parent = 0);
    ~CreatePluginDialog();
    void createPlugin();

private slots:
    void on_chooseDestination_clicked();
    void on_destination_textChanged(const QString &arg1);

    void on_pluginName_textChanged(const QString &arg1);

    void on_filterName_textChanged(const QString &arg1);

    void check();

protected:
    QString readFile(const QString& filename);
    void writeFile(const QString& filename, const QString& data);
private:
    Ui::CreatePluginDialog *ui;
    QString mSourceDir;
};

#endif // CREATEPLUGINDIALOG_H
