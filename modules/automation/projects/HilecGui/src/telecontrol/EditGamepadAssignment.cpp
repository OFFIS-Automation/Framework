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


#include "EditGamepadArea.h"
#include "EditGamepadAssignment.h"
#include "ui_EditGamepadAssignment.h"

#include "../HilecSingleton.h"

#include <core/TelecontrolConfig.h>

#include <QSettings>

EditGamepadAssignment::EditGamepadAssignment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditGamepadAssignment)
{
    ui->setupUi(this);
}

EditGamepadAssignment::~EditGamepadAssignment()
{
    delete ui;
}

void EditGamepadAssignment::load(const QString &unitName, const QString &configFile)
{
    ui->name->setText(unitName);
    QSettings settings(configFile, QSettings::IniFormat);
    int size = settings.beginReadArray("telecontrol-combinations/" + unitName + "/joysticks");
    for(int i=0;i<size;i++)
    {
        settings.setArrayIndex(i);
        QString name = settings.value("name").toString();
        EditGamepadArea* area = addTab(name);
        area->load(unitName, name, configFile);
    }
}

void EditGamepadAssignment::onNameChanged(const QString &text)
{
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), text);
}

void EditGamepadAssignment::on_add_clicked()
{
    QString baseName = "move";
    QString name = baseName;
    int counter = 1;
    bool nameIsOk = false;
    while(!nameIsOk)
    {
        nameIsOk = true;
        for(int i=0;i<ui->tabWidget->count(); i++)
        {
            if(ui->tabWidget->tabText(i) == name)
            {
                nameIsOk = false;
                name = baseName + QString::number(counter++);
                break;
            }
        }
    }
    addTab(name);
}

EditGamepadArea* EditGamepadAssignment::addTab(const QString &name)
{
    EditGamepadArea* area = new EditGamepadArea(name, this);
    ui->tabWidget->addTab(area, name);
    connect(area, SIGNAL(nameChanged(QString)), SLOT(onNameChanged(QString)));
    return area;
}
