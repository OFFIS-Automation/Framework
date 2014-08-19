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


#ifndef EDITGAMEPADASSIGNMENT_H
#define EDITGAMEPADASSIGNMENT_H

#include <QDialog>
#include <QMap>

class EditGamepadArea;
class QComboBox;
class QLineEdit;

namespace Ui {
class EditGamepadAssignment;
}

class EditGamepadAssignment : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditGamepadAssignment(QWidget *parent = 0);
    ~EditGamepadAssignment();
    void load(const QString& unit, const QString& configFile);
    void saveConfig(const QString& configFile);
private slots:
    void onNameChanged(const QString& text);
    void onRemoveCurrentConfig();
    void on_add_clicked();
    void on_remove_clicked();
    void on_current_index_changed(const QString& text);

private:
    EditGamepadArea *addTab(const QString& name);
    Ui::EditGamepadAssignment *ui;
    QString mOldName;
    QMap<int, QComboBox*> mButtonUis;
    QMap<int, QLineEdit*> mButtonNameUis;
    bool mShouldDelete;
};

#endif // EDITGAMEPADASSIGNMENT_H
