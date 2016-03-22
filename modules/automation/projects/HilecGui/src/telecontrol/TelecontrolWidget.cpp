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
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateUnits(bool)));
    connect(HilecSingleton::hilec(), SIGNAL(gamepadUpdated(QString,QString,bool)), SLOT(onTelecontrolUpdated(QString,QString,bool)));
    connect(HilecSingleton::hilec(), SIGNAL(hapticUpdated(QString,QString,bool)), SLOT(onTelecontrolUpdated(QString,QString,bool)));
    connect(HilecSingleton::hilec(), SIGNAL(gamepadSwitchRequested(QString,QString,bool)), SLOT(onGamepadSwitchRequested(QString,QString,bool)));
    connect(this, SIGNAL(updateTelecontrolAssignment(QString,QString)), HilecSingleton::hilec(), SLOT(updateTelecontrolAssignment(QString,QString)), Qt::DirectConnection);
    connect(this, SIGNAL(activateGamepad(QString,QString)), HilecSingleton::hilec(), SLOT(activateGamepad(QString,QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(deactivateGamepadAll()), HilecSingleton::hilec(), SLOT(deactivateGamepadAll()), Qt::QueuedConnection);
    connect(this, SIGNAL(updateGamepadParameters(QString,QString,double, QList<bool>)), HilecSingleton::hilec(), SLOT(updateGamepadParameters(QString,QString,double, QList<bool>)),Qt::QueuedConnection);
    connect(this, SIGNAL(activateHaptic(QString,QString)), HilecSingleton::hilec(), SLOT(activateHaptic(QString,QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(deactivateHapticAll()), HilecSingleton::hilec(), SLOT(deactivateHapticAll()), Qt::QueuedConnection);
    connect(this, SIGNAL(updateHapticParameters(QString,QString,double,double,QList<bool>)), HilecSingleton::hilec(), SLOT(updateHapticParameters(QString,QString,double,double,QList<bool>)),Qt::QueuedConnection);


    mInUpdate = false;
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
    while(ui->tabWidget->count() > 1)
    {
        QWidget* widget = ui->tabWidget->widget(1);
        ui->tabWidget->removeTab(1);
        delete widget;
    }
    ui->tabWidget->setEnabled(false);
}

void TelecontrolWidget::updateUnits(bool /*partialChange */)
{
    clear();
    QStringList units = HilecSingleton::hilec()->getTelecontrolableUnits();
    QStringList rcUnits = HilecSingleton::hilec()->rcUnits();
    foreach(QString unit, units)
    {
        TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(unit);
        if(!help.tcGamepadMoves.empty() || !help.tcButtonMethods.empty() || !help.tcHapticMoves.empty()){
            ui->tabWidget->setEnabled(true);

            // Setup a layout container
            QWidget* page = new QWidget;
            QVBoxLayout* layout = new QVBoxLayout();
            page->setLayout(layout);

            // Add device selection combobox
            QStringList devices;
            devices << HilecSingleton::hilec()->getGamepadDevices().keys();
            devices << HilecSingleton::hilec()->getHapticDevices().keys();

            TelecontrolSelectionComboBox *comboBox = new TelecontrolSelectionComboBox(unit, devices, help.tcDeviceName);
            connect(comboBox, SIGNAL(telecontrolSelected(QString,QString)), this, SLOT(onTelecontrolAssignmentUpdate(QString,QString)));
            layout->addWidget(comboBox);

            // Add assignment button
            ShowAssignmentButton *button = new ShowAssignmentButton(unit);
            connect(button, SIGNAL(openButtonAssignment(QString)), this, SLOT(openButtonAssignment(QString)));
            connect(button, SIGNAL(editButtonAssignment(QString)), this, SLOT(editButtonAssignment(QString)));
            connect(comboBox, SIGNAL(telecontrolSelected(QString,QString)), button, SLOT(updateTelecontrolAssignment(QString,QString)));
            layout->addWidget(button);

            // Add slider, gain, .. for each method
            foreach(RcUnitHelp::TcMove method, help.tcGamepadMoves){
                TelecontrolGamepadWidget* gamepadWidget = new TelecontrolGamepadWidget(unit, method);
                connect(gamepadWidget, SIGNAL(updateGamepadParameters(QString,QString,double,QList<bool>)), SIGNAL(updateGamepadParameters(QString,QString,double, QList<bool>)));
                layout->addWidget(gamepadWidget);
                connect(HilecSingleton::hilec(), SIGNAL(gamepadSensitivityChangeRequested(QString,QString,bool)), gamepadWidget, SLOT(changeSlider(QString,QString,bool)));
            }

            // Add slider, gain, .. for each method
            foreach(RcUnitHelp::TcMove method, help.tcHapticMoves){
                TelecontrolHapticWidget* hapticWidget = new TelecontrolHapticWidget(unit, method);
                connect(hapticWidget, SIGNAL(updateHapticParameters(QString,QString,double,double,QList<bool>)), SIGNAL(updateHapticParameters(QString,QString,double,double,QList<bool>)));
                layout->addWidget(hapticWidget);
            }

            layout->addStretch(1);
            int index = ui->tabWidget->addTab(page, QIcon(":/hilecGui/controller.png"), unit);
            mUnitIndexes[index] = unit;
        }
    }
}


void TelecontrolWidget::on_tabWidget_currentChanged(int index)
{
    if(mInUpdate){
        return;
    }
    emit deactivateGamepadAll();
    emit deactivateHapticAll();
    QString unit = mUnitIndexes.value(index);
    if(unit.length() > 0){
        TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(unit);
        emit activateGamepad(help.tcDeviceName, unit);
        emit activateHaptic(help.tcDeviceName, unit);
    }
}

void TelecontrolWidget::onTelecontrolAssignmentUpdate(const QString& deviceName, const QString& unitName)
{
    // Emit to inform core system, then update GUI
    emit updateTelecontrolAssignment(deviceName, unitName);
    onTelecontrolUpdated(deviceName, unitName, true);
}

void TelecontrolWidget::onTelecontrolUpdated(const QString &deviceName, const QString &unitName, bool gamepadActive)
{    
    mInUpdate = true;

    if(gamepadActive){
        int id = mUnitIndexes.key(unitName,0);
        ui->tabWidget->setCurrentIndex(id);

        // Remove mHapticWidget
        QWidget *tabWidget = ui->tabWidget->currentWidget();
        QBoxLayout *layout = (QBoxLayout *)tabWidget->layout();
        if(tabWidget->children().contains(mHapticWidget)){
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

        QStringList hapticDevices = HilecSingleton::hilec()->getHapticDevices().keys();
        if(hapticDevices.contains(deviceName)){
            QWidget *hapticWidget = HilecSingleton::hilec()->createHapticWidget(unitName);
            // Store refernce for later usage
            mHapticWidget = hapticWidget;
            layout->insertWidget(2, mHapticWidget);
        }
    }

    mInUpdate = false;
}

void TelecontrolWidget::onGamepadSwitchRequested(const QString& deviceName, const QString& unitName, bool down)
{
    Q_UNUSED(deviceName);

    int currentIndex = mUnitIndexes.key(unitName, 0);
    int newIndex = down? currentIndex-1 : currentIndex+1;
    int minIndex = 0; int maxIndex = ui->tabWidget->count() - 1;

    // Cycle through the list
    if(newIndex < minIndex){
        newIndex = maxIndex;
    } else if (newIndex > maxIndex){
        newIndex = minIndex;
    }

    // Set index
    ui->tabWidget->setCurrentIndex(newIndex);
}

void TelecontrolWidget::editButtonAssignment(const QString &unitName)
{
    Q_UNUSED(unitName);
    /*
    EditGamepadAssignment edit(this);
    if(!unit.isEmpty())
        edit.load(unit, mConfigFile);
    if(edit.exec())
    {
        edit.saveConfig(mConfigFile);
        QMessageBox::information(this, tr("Reload required"), tr("You need to reload your project for changes to take effect"), QMessageBox::Ok);
    }
    */
}

void TelecontrolWidget::openButtonAssignment(const QString &unitName)
{
    if(!mTelecontrolAssignmentWidget){
        mTelecontrolAssignmentWidget = new TelecontrolAssignmentWidget(this);
    }
    mTelecontrolAssignmentWidget->switchToUnit(unitName);
    mTelecontrolAssignmentWidget->show();

}
