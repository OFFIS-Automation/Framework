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
#include "ui_EditGamepadArea.h"
#include "../HilecSingleton.h"

#include <telecontrol/TcConfig.h>

#include <QSettings>

EditGamepadArea::EditGamepadArea(const QString &name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditGamepadArea)
{
    ui->setupUi(this);
    ui->name->setText(name);

    QString label = "not assigned";

    mJoystickUis[Tc::LeftJoystickX] = ui->leftJoystickX;
    mJoystickUis[Tc::LeftJoystickY] = ui->leftJoystickY;
    mJoystickUis[Tc::RightJoystickX] = ui->rightJoystickX;
    mJoystickUis[Tc::RightJoystickY] = ui->rightJoystickY;

    foreach(QComboBox* cb, mJoystickUis.values())
        cb->addItem(label);

    for(int i=Tc::ButtonEnumFirst; i<Tc::ButtonEnumEnd; i++)
    {
        ui->deadMansButton->addItem(Tc::userFriendlyStringForButton(i), i);
    }
    ui->deadMansButton->setCurrentIndex(ui->deadMansButton->findData(Tc::LeftShoulderUpperButton));

    foreach(const QString& unit, HilecSingleton::hilec()->rcUnits())
    {
        TelecontrolConfig tcConfig = HilecSingleton::hilec()->getTelecontrolConfig(unit);
        foreach(const TelecontrolConfig::TcJostick& joystick, tcConfig.tcJoysticks)
        {
            foreach(QString param, joystick.axeNames)
            {
                label = unit + "." + joystick.name + "." + param;
                foreach(QComboBox* cb, mJoystickUis.values())
                    cb->addItem(label);
            }
        }
    }

    connect(ui->name, SIGNAL(textChanged(QString)), SIGNAL(nameChanged(QString)));
    connect(ui->deleteThis, SIGNAL(clicked()), SIGNAL(removeCurrentConfig()));
}

EditGamepadArea::~EditGamepadArea()
{
    delete ui;
}

void EditGamepadArea::load(const QString& unitName, const QString& name, const QString& configFile)
{
    QSettings settings(configFile, QSettings::IniFormat);
    int size = settings.beginReadArray("telecontrol-combinations/" + unitName + "/joysticks");
    for(int i=0;i<size;i++)
    {
        settings.setArrayIndex(i);
        if(settings.value("name").toString() == name)
        {
            QString activationButtonStr = settings.value("activationButton").toString();
            Tc::Button activationButton = Tc::buttonFromString(activationButtonStr);
            ui->deadMansButton->setCurrentIndex(ui->deadMansButton->findData(activationButton));
            foreach(Tc::Joystick id, Tc::allJoysticks())
            {
                settings.beginGroup(Tc::stringForJoystick(id));
                QString unit = settings.value("unit").toString();
                QString method = settings.value("method").toString();
                QString channel = settings.value("channel").toString();
                QString label = unit + "." + method + "." + channel;
                QComboBox* cb = mJoystickUis.value(id, 0);
                if(cb)
                {
                    int index = cb->findText(label);
                    if(index >= 0)
                        cb->setCurrentIndex(index);
                }
                settings.endGroup();
            }
        }
    }

}

void EditGamepadArea::saveConfig(QSettings &settings)
{
    settings.setValue("name", ui->name->text());
    int index = ui->deadMansButton->currentIndex();
    settings.setValue("activationButton", Tc::stringForButton(ui->deadMansButton->itemData(index).toInt()));
}
