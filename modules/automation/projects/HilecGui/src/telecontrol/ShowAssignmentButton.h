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

#ifndef SHOWASSIGNMENTBUTTON_H
#define SHOWASSIGNMENTBUTTON_H

#include <QWidget>

namespace Ui {
class ShowAssignmentButton;
}

class ShowAssignmentButton : public QWidget
{
    Q_OBJECT
    
public:
    explicit ShowAssignmentButton(QString unitName);
    ~ShowAssignmentButton();

public slots:
    void updateTelecontrolAssignment(const QString &unitName, const QString& telecontrolDeviceName);

private slots:
    void on_show_clicked();
    void on_edit_clicked();

signals:
    void openButtonAssignment(QString unit);
    void editButtonAssignment(QString unit);

private:
    QString mUnitName;
    Ui::ShowAssignmentButton *ui;
    void changeButtons(const QString &telecontrolDeviceName);
};

#endif // SHOWASSIGNMENTBUTTON_H
