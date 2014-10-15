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

#include "TelecontrolWidget.h"
#include "ui_TelecontrolWidget.h"
#include <core/RcUnitHelp.h>
#include <QDebug>
#include <QFormLayout>
#include <QCheckBox>
#include "TelecontrolGamepadWidget.h"
#include "TelecontrolHapticWidget.h"
#include "ShowAssignmentButton.h"
#include "EditGamepadAssignment.h"
#include "../HilecSingleton.h"
#include <QMessageBox>
#include <cmath>

TelecontrolWidget::TelecontrolWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TelecontrolWidget)
{
    ui->setupUi(this);
    ui->gamepadTabWidget->setEnabled(false);
    ui->hapticGroupBox->setVisible(false);
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateUnits(bool)));
    connect(HilecSingleton::hilec(), SIGNAL(telecontrolUpdated(bool, QString)), SLOT(onTelecontrolUpdated(bool, QString)));
    connect(HilecSingleton::hilec(), SIGNAL(hapticUpdated(bool, QString)), SLOT(onHapticUpdated(bool, QString)));

    connect(this, SIGNAL(activateTelecontrol(QString)), HilecSingleton::hilec(), SLOT(activateTelecontrol(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(updateTelecontrol(QString,QString,double, QList<bool>)), HilecSingleton::hilec(), SLOT(updateTelecontrol(QString,QString,double, QList<bool>)),Qt::QueuedConnection);
    connect(this, SIGNAL(deactivateTelecontrol()), HilecSingleton::hilec(), SLOT(deactivateTelecontrol()), Qt::QueuedConnection);

    connect(this, SIGNAL(activateHaptic(QString)), HilecSingleton::hilec(), SLOT(activateHaptic(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(updateHaptic(QString,QString,double,double,QList<bool>)), HilecSingleton::hilec(), SLOT(updateHaptic(QString,QString,double,double,QList<bool>)),Qt::QueuedConnection);
    connect(this, SIGNAL(deactivateHaptic()), HilecSingleton::hilec(), SLOT(deactivateHaptic()), Qt::QueuedConnection);

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
    while(ui->gamepadTabWidget->count() > 1)
    {
        QWidget* widget = ui->gamepadTabWidget->widget(1);
        ui->gamepadTabWidget->removeTab(1);
        delete widget;
    }
    while(ui->hapticTabWidget->count() > 1)
    {
        QWidget* widget = ui->hapticTabWidget->widget(1);
        ui->hapticTabWidget->removeTab(1);
        delete widget;
    }
    ui->gamepadTabWidget->setEnabled(false);
    ui->hapticGroupBox->setVisible(false);
}

void TelecontrolWidget::updateUnits(bool /*partialChange */)
{
    clear();
    QStringList units = HilecSingleton::hilec()->getTelecontrolableUnits();
    QStringList rcUnits = HilecSingleton::hilec()->rcUnits();
    foreach(QString unit, units)
    {
        TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(unit);
        if(!help.tcGamepadMoves.empty() || !help.tcGamepadButtons.empty())
        {
            ui->gamepadTabWidget->setEnabled(true);

            // Setup a layout container
            QWidget* page = new QWidget;
            QVBoxLayout* layout = new QVBoxLayout();
            page->setLayout(layout);

            // Add assignment button
            ShowAssignmentButton *button = new ShowAssignmentButton(unit, rcUnits.contains(unit));
            connect(button, SIGNAL(openButtonAssignment(QString)), this, SLOT(on_openButtonAssignment_clicked(QString)));
            connect(button, SIGNAL(editButtonAssignment(QString)), this, SLOT(editButtonAssignment(QString)));
            layout->addWidget(button);

            // Add slider, gain, .. for each method
            foreach(RcUnitHelp::TcMove method, help.tcGamepadMoves){
                TelecontrolGamepadWidget* gamepadWidget = new TelecontrolGamepadWidget(unit, method);
                connect(gamepadWidget, SIGNAL(updateTelecontrol(QString,QString,double,QList<bool>)), SIGNAL(updateTelecontrol(QString,QString,double, QList<bool>)));
                layout->addWidget(gamepadWidget);
                connect(HilecSingleton::hilec(), SIGNAL(telecontrolChangeSensitivityRequested(QString,bool)), gamepadWidget, SLOT(changeSlider(QString,bool)));
            }
            layout->addStretch(1);
            int index = ui->gamepadTabWidget->addTab(page, QIcon(":/hilecGui/controller.png"), unit);
            mUnitIndexes[index] = unit;
        }
        if(!help.tcHapticMoves.empty() || !help.tcHapticButtons.empty())
        {
            ui->hapticGroupBox->setVisible(true);
            ui->hapticTabWidget->setEnabled(true);

            // Setup a layout container
            QWidget* page = new QWidget;
            QVBoxLayout* layout = new QVBoxLayout();
            page->setLayout(layout);

            // Add slider, gain, .. for each method
            foreach(RcUnitHelp::TcMove method, help.tcHapticMoves){
                TelecontrolHapticWidget* hapticWidget = new TelecontrolHapticWidget(unit, method);
                connect(hapticWidget, SIGNAL(updateHaptic(QString,QString,double,double,QList<bool>)), SIGNAL(updateHaptic(QString,QString,double,double,QList<bool>)));
                layout->addWidget(hapticWidget);
            }

            layout->addStretch(1);
            int index = ui->hapticTabWidget->addTab(page, QIcon(":/hilecGui/joystick.png"), unit);
            mUnitIndexes[sHapticIndexOffset + index] = unit;
        }
    }
}


void TelecontrolWidget::on_gamepadTabWidget_currentChanged(int index)
{
    if(mInUpdate)
        return;
    QString unit = mUnitIndexes.value(index);
    emit activateTelecontrol(unit);
}

void TelecontrolWidget::onTelecontrolUpdated(bool active, const QString &activeUnit)
{
    mInUpdate = true;

    if(active){
        int id = mUnitIndexes.key(activeUnit,0);
        ui->gamepadTabWidget->setCurrentIndex(id);
    }

    mInUpdate = false;
}

void TelecontrolWidget::onHapticUpdated(bool active, const QString &activeUnit)
{
    mInUpdate = true;

    if(active){
        int id = mUnitIndexes.key(activeUnit,0);
        if (id > sHapticIndexOffset){
            id -= sHapticIndexOffset;
        }
        ui->hapticTabWidget->setCurrentIndex(id);
    }
    mInUpdate = false;
    if(active && !mHapticWidget){
        mHapticWidget = HilecSingleton::hilec()->createHapticWidget();
        if(mHapticWidget){
            ui->hapticLayout->insertWidget(ui->hapticLayout->indexOf(ui->hapticTabWidget), mHapticWidget);
        }
    }
    if(!active && mHapticWidget){
        ui->hapticLayout->removeWidget(mHapticWidget);
        delete mHapticWidget;
        mHapticWidget = 0;
    }
}

void TelecontrolWidget::on_hapticTabWidget_currentChanged(int index)
{
    if(mInUpdate){
        return;
    }
    QString unit = mUnitIndexes.value(sHapticIndexOffset + index);
    emit activateHaptic(unit);
}

void TelecontrolWidget::on_openButtonAssignment_clicked(QString unit)
{
    if(!mTelecontrolAssignmentWidget){
        mTelecontrolAssignmentWidget = new TelecontrolAssignmentWidget(this);
    }
    mTelecontrolAssignmentWidget->switchToUnit(unit);
    mTelecontrolAssignmentWidget->show();

}

void TelecontrolWidget::editButtonAssignment(const QString &unit)
{
    EditGamepadAssignment edit(this);
    if(!unit.isEmpty())
        edit.load(unit, mConfigFile);
    if(edit.exec())
    {
        edit.saveConfig(mConfigFile);
        QMessageBox::information(this, tr("Relaod required"), tr("You need to reload your project for changes to take effect"), QMessageBox::Ok);
    }
}


