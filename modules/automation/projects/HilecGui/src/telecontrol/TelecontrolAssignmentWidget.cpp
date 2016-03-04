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

#include "TelecontrolAssignmentWidget.h"
#include "ui_TelecontrolAssignmentWidget.h"

#include "GamepadAssignmentWidget.h"
#include "ConnexionAssignmentWidget.h"

#include <core/RcUnitHelp.h>
#include "../HilecSingleton.h"

QStringList TelecontrolAssignmentWidget::connexionControllerNames = QStringList() << "SpaceNavigator" << "SpaceNavigator for Notebooks"  << "SpaceTraveler USB" << "SpaceExplorer" << "SpacePilot";

TelecontrolAssignmentWidget::TelecontrolAssignmentWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelecontrolAssignmentWidget)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowTitle(tr("Telecontrol assignment"));
    ui->setupUi(this);

    // Signal / Slot stuff
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateUnits(bool)));
    connect(HilecSingleton::hilec(), SIGNAL(gamepadUpdated(QString,QString,bool)), SLOT(onTelecontrolUpdated(QString,QString,bool)));
    connect(HilecSingleton::hilec(), SIGNAL(hapticUpdated(QString,QString,bool)), SLOT(onTelecontrolUpdated(QString,QString,bool)));

    // Init view
    updateUnits(true);

    // Hide help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

TelecontrolAssignmentWidget::~TelecontrolAssignmentWidget()
{
    delete ui;
}

void TelecontrolAssignmentWidget::clear()
{
    // Remove stored indices
    mUnitIndexes.clear();
    // Remove all tabs
    while(ui->tabWidget->count() > 0){
        QWidget* widget = ui->tabWidget->widget(0);
        ui->tabWidget->removeTab(0);
        delete widget;
    }
    // Disable units
    ui->tabWidget->setEnabled(false);
}

void TelecontrolAssignmentWidget::updateUnits(bool /*partialReload*/)
{
    // Clear view
    clear();
    // Reload units
    QStringList units = HilecSingleton::hilec()->getTelecontrolableUnits();
    foreach(QString unit, units){
        TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(unit);
        // Check if uni has gamepad methods
        if(help.tcGamepadMoves.empty() && help.tcButtonMethods.empty()){
            continue;
        }

        ui->tabWidget->setEnabled(true);

        // Create tab
        QWidget* page = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout();
        page->setLayout(layout);
        if(connexionControllerNames.contains(help.tcDeviceName)){
            layout->addWidget(new ConnexionAssignmentWidget(unit, this));
        } else {
            layout->addWidget(new GamepadAssignmentWidget(unit, this));
        }

        // Store index in QMap for later usage
        int index = ui->tabWidget->addTab(page, QIcon(":/hilecGui/controller_info.png"), unit);
        mUnitIndexes[index] = unit;
    }
}

void TelecontrolAssignmentWidget::switchToUnit(QString unit)
{
    updateUnits(false);
    int id = mUnitIndexes.key(unit, -1);
    if(id >= 0)
        ui->tabWidget->setCurrentIndex(id);
}

void TelecontrolAssignmentWidget::onTelecontrolUpdated(const QString &deviceName, const QString &unitName, bool active)
{
    if(active){
        int id = mUnitIndexes.key(unitName,0);
        ui->tabWidget->setEnabled(true);
        ui->tabWidget->setCurrentIndex(id);
    }
}
