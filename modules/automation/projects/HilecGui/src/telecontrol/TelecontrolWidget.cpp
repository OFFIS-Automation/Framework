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

#include "TelecontrolWidget.h"
#include "ui_TelecontrolWidget.h"

// QT headers
#include <QDebug>
#include <QFormLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <cmath>

// Project releated headers
#include "TelecontrolGamepadWidget.h"
#include "TelecontrolHapticWidget.h"
#include "ShowAssignmentButton.h"
#include "TelecontrolSelectionComboBox.h"
#include "EditGamepadAssignment.h"
#include "../HilecSingleton.h"
#include <core/RcUnitHelp.h>

TelecontrolWidget::TelecontrolWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TelecontrolWidget)
{
    ui->setupUi(this);
    ui->tabWidget->setEnabled(false);

    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(onRcUnitsChanged(bool)));
    connect(HilecSingleton::hilec(), SIGNAL(gamepadSwitchRequested(QString,QString,bool)), SLOT(onGamepadSwitchRequested(QString,QString,bool)));

    connect(this, SIGNAL(updateTelecontrolAssignment(QString,QString)), HilecSingleton::hilec(), SLOT(updateTelecontrolAssignment(QString,QString)), Qt::DirectConnection);
    connect(this, SIGNAL(activateGamepad(QString,QString)), HilecSingleton::hilec(), SLOT(activateGamepad(QString,QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(deactivateGamepadAll()), HilecSingleton::hilec(), SLOT(deactivateGamepadAll()), Qt::QueuedConnection);
    connect(this, SIGNAL(updateGamepadParameters(QString,QString,double, QList<bool>)), HilecSingleton::hilec(), SLOT(updateGamepadParameters(QString,QString,double, QList<bool>)),Qt::QueuedConnection);
    connect(this, SIGNAL(activateHaptic(QString,QString)), HilecSingleton::hilec(), SLOT(activateHaptic(QString,QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(deactivateHapticAll()), HilecSingleton::hilec(), SLOT(deactivateHapticAll()), Qt::QueuedConnection);
    connect(this, SIGNAL(updateHapticParameters(QString,QString,double,double,QList<bool>)), HilecSingleton::hilec(), SLOT(updateHapticParameters(QString,QString,double,double,QList<bool>)),Qt::QueuedConnection);

    mHapticWidget = 0;
    mTelecontrolAssignmentWidget = 0;
}

TelecontrolWidget::~TelecontrolWidget()
{
    delete ui;
}

void TelecontrolWidget::clear()
{
    mUnitIndexes.clear();
    mUnitIndexes[0] = "";

    // Remove all tabs
    while(ui->tabWidget->count() > 1){
        QWidget* widget = ui->tabWidget->widget(1);
        ui->tabWidget->removeTab(1);
        delete widget;
    }
    ui->tabWidget->setEnabled(false);
}

void TelecontrolWidget::onRcUnitsChanged(bool /*partialChange */)
{
    QStringList units = HilecSingleton::hilec()->getTelecontrolableUnits();
    if(mTelecontrolUnits == units){
        return;
    }
    mTelecontrolUnits = units;
    clear();

    foreach(QString unit, mTelecontrolUnits){
        // Check if unit is telecontrolable at all
        TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(unit);
        if(!help.tcGamepadMoves.empty() || !help.tcButtonMethods.empty() || !help.tcHapticMoves.empty()){
            // Setup a layout container
            QWidget* page = new QWidget;
            QVBoxLayout* layout = new QVBoxLayout();
            page->setLayout(layout);

            // Add device selection combobox
            QStringList devices;
            devices << HilecSingleton::hilec()->getGamepadDevices().keys();
            devices << HilecSingleton::hilec()->getHapticDevices().keys();

            TelecontrolSelectionComboBox *comboBox = new TelecontrolSelectionComboBox(unit, devices, help.tcDeviceName);
            layout->addWidget(comboBox);

            // Add assignment button
            ShowAssignmentButton *button = new ShowAssignmentButton(unit);
            layout->addWidget(button);

            // Signal / slots
            connect(comboBox, SIGNAL(telecontrolSelected(QString,QString)), this, SLOT(onTelecontrolAssignmentUpdate(QString,QString)));
            connect(comboBox, SIGNAL(telecontrolSelected(QString,QString)), button, SLOT(updateTelecontrolAssignment(QString,QString)));
            connect(button, SIGNAL(openButtonAssignment(QString)), this, SLOT(openButtonAssignment(QString)));
            connect(button, SIGNAL(editButtonAssignment(QString)), this, SLOT(editButtonAssignment(QString)));

            // Add slider, gain, .. for each method
            foreach(RcUnitHelp::TcMove method, help.tcGamepadMoves){
                TelecontrolGamepadWidget* gamepadWidget = new TelecontrolGamepadWidget(unit, method);
                connect(gamepadWidget, SIGNAL(updateGamepadParameters(QString,QString,double,QList<bool>)), SIGNAL(updateGamepadParameters(QString,QString,double, QList<bool>)));
                connect(HilecSingleton::hilec(), SIGNAL(gamepadSensitivityChangeRequested(QString,QString,bool)), gamepadWidget, SLOT(changeSlider(QString,QString,bool)));
                layout->addWidget(gamepadWidget);
            }

            // Add slider, gain, .. for each method
            foreach(RcUnitHelp::TcMove method, help.tcHapticMoves){
                TelecontrolHapticWidget* hapticWidget = new TelecontrolHapticWidget(unit, method);
                connect(hapticWidget, SIGNAL(updateHapticParameters(QString,QString,double,double,QList<bool>)), SIGNAL(updateHapticParameters(QString,QString,double,double,QList<bool>)));
                layout->addWidget(hapticWidget);
            }

            layout->addStretch(1);

            // Store index for later usage
            int index = ui->tabWidget->addTab(page, QIcon(":/hilecGui/controller.png"), unit);
            mUnitIndexes[index] = unit;
        }
    }

    // Check if tabWidget should be enabled, at least one unit
    ui->tabWidget->setEnabled(ui->tabWidget->count() > 1);
}

void TelecontrolWidget::editButtonAssignment(const QString &unitName)
{
    Q_UNUSED(unitName);
    /*EditGamepadAssignment *editGamepadAssignment = new EditGamepadAssignment(this);
    if(!unit.isEmpty()){
        editGamepadAssignment.load(unit, mConfigFile);
    }
    if(editGamepadAssignment.exec()){
        editGamepadAssignment.saveConfig(mConfigFile);
        QMessageBox::information(this, tr("Reload required"), tr("You need to reload your project for changes to take effect"), QMessageBox::Ok);
    }*/
}

void TelecontrolWidget::openButtonAssignment(const QString &unitName)
{
    if(!mTelecontrolAssignmentWidget){
        mTelecontrolAssignmentWidget = new TelecontrolAssignmentWidget(this);
    }
    mTelecontrolAssignmentWidget->switchToUnit(unitName);
    mTelecontrolAssignmentWidget->show();
}

void TelecontrolWidget::onTelecontrolAssignmentUpdate(const QString& deviceName, const QString& unitName)
{
    // Emit to inform core system, then update GUI
    emit updateTelecontrolAssignment(deviceName, unitName);
}

void TelecontrolWidget::onGamepadSwitchRequested(const QString& deviceName, const QString& unitName, bool down)
{
    Q_UNUSED(deviceName);

    int currentIndex = mUnitIndexes.key(unitName, 0);
    int newIndex = down ? currentIndex+1 : currentIndex-1;

    // Bound to range
    newIndex = qBound<int>(0, newIndex, ui->tabWidget->count()-1);

    // Set index
    ui->tabWidget->setCurrentIndex(newIndex);
}

void TelecontrolWidget::on_tabWidget_currentChanged(int index)
{
    // Deactivate all previous gamepads
    emit deactivateGamepadAll();
    emit deactivateHapticAll();


    QString unitName = mUnitIndexes.value(index);
    if(!unitName.isEmpty()){
        // Update GUI: Remove a haptic widget which is might on the page
        QWidget *page = ui->tabWidget->currentWidget();
        QBoxLayout *layout = (QBoxLayout *)page->layout();
        if(page->children().contains(mHapticWidget)){
            unsigned int i = 0;
            while (QLayoutItem* item = layout->itemAt(i)){
                if (item->widget() == mHapticWidget){
                    layout->removeItem(item);
                    item->widget()->deleteLater();
                    mHapticWidget = 0;
                    break;
                } else {
                    i++;
                }
            }
        }

        // Activate the new unit, if not "disabled tab"
        TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(unitName);
        // Check if the tcDevice is a gamepad
        QStringList gamepadDevices = HilecSingleton::hilec()->getGamepadDevices().keys();
        if(gamepadDevices.contains(help.tcDeviceName)){
            emit activateGamepad(help.tcDeviceName, unitName);
        } else {
             // Check if the tcDevice is a haptic device
            QStringList hapticDevices = HilecSingleton::hilec()->getHapticDevices().keys();
            if(hapticDevices.contains(help.tcDeviceName)){
                emit activateHaptic(help.tcDeviceName, unitName);

                // In addition: Add haptic widget to the page
                QWidget *hapticWidget = HilecSingleton::hilec()->createHapticWidget(unitName);
                // Store refernce for later usage
                mHapticWidget = hapticWidget;
                layout->insertWidget(2, mHapticWidget);
            }
        }
    }
}
