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


#include "EditGamepadArea.h"
#include "EditGamepadAssignment.h"
#include "ui_EditGamepadAssignment.h"

#include "../HilecSingleton.h"

#include <core/TelecontrolConfig.h>

#include <QComboBox>
#include <QSettings>

EditGamepadAssignment::EditGamepadAssignment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditGamepadAssignment)
{
    mShouldDelete = false;
    ui->setupUi(this);
    ui->remove->setEnabled(false);
    int rowCounter = 1;
    for(int i=Tc::ButtonEnumFirst; i<Tc::ButtonEnumEnd; i++)
    {
        QComboBox* box = new QComboBox();
        QLineEdit* edit = new QLineEdit();
        box->addItem("Not assigned");
        mButtonUis[i] = box;
        mButtonNameUis[i] = edit;
        QString label = Tc::userFriendlyStringForButton(i);
        ui->buttonTabLayout->addWidget(new QLabel(label),rowCounter, 0);
        ui->buttonTabLayout->addWidget(edit,rowCounter, 1);
        ui->buttonTabLayout->addWidget(box,rowCounter, 2);
        rowCounter++;
    }
    foreach(const QString& unit, HilecSingleton::hilec()->rcUnits())
    {
        TelecontrolConfig tcConfig = HilecSingleton::hilec()->getTelecontrolConfig(unit);
        foreach(const TelecontrolConfig::TcButton& button, tcConfig.tcButtons)
        {
            QString label = unit + "." + button.name;
            foreach(QComboBox* cb, mButtonUis.values())
                cb->addItem(label);
        }
    }
}

EditGamepadAssignment::~EditGamepadAssignment()
{
    delete ui;
}

void EditGamepadAssignment::load(const QString &unitName, const QString &configFile)
{
    ui->remove->setEnabled(true);
    mOldName = unitName;
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
    settings.endArray();
    size = settings.beginReadArray("telecontrol-combinations/" + unitName + "/buttons");
    for(int i=0;i<size;i++)
    {
        settings.setArrayIndex(i);
        QString buttonName = settings.value("button").toString();
        QString name = settings.value("name").toString();
        QString unit =settings.value("unit").toString();
        QString method =settings.value("method").toString();
        QString label = unit + "." + method;
        Tc::Button button = Tc::buttonFromString(buttonName);
        QComboBox* cb = mButtonUis.value(button, 0);
        if(cb)
        {
            int index = cb->findText(label);
            if(index >= 0)
                cb->setCurrentIndex(index);
        }
        QLineEdit* nameEdit = mButtonNameUis.value(button, 0);
        if(nameEdit)
            nameEdit->setText(name);

    }
    settings.endArray();
}

void EditGamepadAssignment::saveConfig(const QString &configFile)
{
    QSettings settings(configFile, QSettings::IniFormat);
    settings.beginGroup("telecontrol-combinations");
    if(!mOldName.isEmpty())
    {
        settings.remove(mOldName);
        if(mShouldDelete)
            return;
    }
    const QString& name = ui->name->text();
    settings.remove(name);
    settings.beginGroup(name);
    settings.beginWriteArray("joysticks");
    for(int i=1;i<ui->tabWidget->count();i++)
    {
        settings.setArrayIndex(i-1);
        EditGamepadArea* area = qobject_cast<EditGamepadArea*>(ui->tabWidget->widget(i));
        Q_ASSERT(area != 0);
        area->saveConfig(settings);
    }
    settings.endArray();
    settings.beginWriteArray("buttons");
    int id = 0;
    foreach(int buttonId, mButtonNameUis.keys())
    {
        QComboBox* cb = mButtonUis.value(buttonId);
        QStringList labelParts = cb->currentText().split(".");
        if(labelParts.size() != 2)
            continue;
        settings.setArrayIndex(id++);
        settings.setValue("button", Tc::stringForButton(buttonId));
        settings.setValue("unit", labelParts[0]);
        settings.setValue("method", labelParts[1]);
        QLineEdit* nameEdit = mButtonNameUis.value(buttonId, 0);
        Q_ASSERT(nameEdit != 0);
        settings.setValue("name", nameEdit->text());
    }
    settings.endArray();
}

void EditGamepadAssignment::onNameChanged(const QString &text)
{
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), text);
}

void EditGamepadAssignment::onRemoveCurrentConfig()
{
    QWidget* w = ui->tabWidget->currentWidget();
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    w->deleteLater();
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
    QWidget* w = addTab(name);
    ui->tabWidget->setCurrentWidget(w);
}

EditGamepadArea* EditGamepadAssignment::addTab(const QString &name)
{
    EditGamepadArea* area = new EditGamepadArea(name, this);
    ui->tabWidget->addTab(area, name);
    connect(area, SIGNAL(nameChanged(QString)), SLOT(onNameChanged(QString)));
    connect(area, SIGNAL(removeCurrentConfig()), SLOT(onRemoveCurrentConfig()));
    return area;
}

void EditGamepadAssignment::on_remove_clicked()
{
    mShouldDelete = true;
    emit accept();
}
