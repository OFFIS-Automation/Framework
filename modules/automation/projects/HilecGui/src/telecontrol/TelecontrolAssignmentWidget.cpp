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

#include "TelecontrolAssignmentWidget.h"
#include "ui_TelecontrolAssignmentWidget.h"

#include "GamepadAssignmentWidget.h"
#include <core/RcUnitHelp.h>
#include "../HilecSingleton.h"

TelecontrolAssignmentWidget::TelecontrolAssignmentWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelecontrolAssignmentWidget)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowTitle(tr("Gamepad button assignment"));
    ui->setupUi(this);

    // Signal / Slot stuff
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateUnits(bool)));
    connect(HilecSingleton::hilec(), SIGNAL(telecontrolUpdated(bool, QString)), SLOT(onTelecontrolUpdated(bool, QString)));

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
        if(help.tcJoysticks.empty() && help.tcButtons.empty())
            continue;

        ui->tabWidget->setEnabled(true);
        // Create tab
        QWidget* page = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout();
        page->setLayout(layout);
        layout->addWidget(new GamepadAssignmentWidget(unit, this));
        int index = ui->tabWidget->addTab(page, QIcon(":/hilecGui/controller_info.png"), unit);
        // Store index in QMap for later usage
        mUnitIndexes[index] = unit;
    }
}

void TelecontrolAssignmentWidget::switchToUnit(QString unit)
{
    int id = mUnitIndexes.key(unit, -1);
    if(id >= 0)
        ui->tabWidget->setCurrentIndex(id);
}

void TelecontrolAssignmentWidget::onTelecontrolUpdated(bool active, const QString &activeUnit)
{
    if(active){
        int id = mUnitIndexes.key(activeUnit,0);
        ui->tabWidget->setEnabled(true);
        ui->tabWidget->setCurrentIndex(id);
    }
}
