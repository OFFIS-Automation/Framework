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

#include "TelecontrolHapticWidget.h"
#include "ui_TelecontrolHapticWidget.h"
#include "TelecontrolUnitWidget.h"
#include <math.h>

TelecontrolHapticWidget::TelecontrolHapticWidget(QString unit, double sensitivity, double forceFactor, QWidget *parent) :
    mUnit(unit),
    QWidget(parent),
    ui(new Ui::TelecontrolHapticWidget)
{
    ui->setupUi(this);
    ui->positionSlider->setRange(0, TelecontrolUnitWidget::numSteps);
    ui->positionSlider->setValue(0);
    for(int i=TelecontrolUnitWidget::numSteps; i>0; i--)
    {
        if( (pow(2.0, i) / pow(2.0, TelecontrolUnitWidget::numSteps)) <= sensitivity)
        {
            ui->positionSlider->setValue(i);
            break;
        }
    }
    ui->forceSlider->setRange(0, TelecontrolUnitWidget::numSteps);
    ui->forceSlider->setValue(0);
    for(int i=TelecontrolUnitWidget::numSteps; i>0; i--)
    {
        if( (pow(2.0, i) / pow(2.0, TelecontrolUnitWidget::numSteps)) <= forceFactor)
        {
            ui->forceSlider->setValue(i);
            break;
        }
    }
}

TelecontrolHapticWidget::~TelecontrolHapticWidget()
{
    delete ui;
}

void TelecontrolHapticWidget::sendHapticUpdate()
{
    double sensitivity = pow(2.0, ui->positionSlider->value()) / pow(2.0, TelecontrolUnitWidget::numSteps);
    double forceFactor  = pow(2.0, ui->forceSlider->value()) / pow(2.0, TelecontrolUnitWidget::numSteps);
    emit updateHaptic(mUnit, sensitivity, forceFactor);
}

void TelecontrolHapticWidget::on_positionSlider_valueChanged(int)
{
    sendHapticUpdate();
}
