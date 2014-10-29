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

#include "TelecontrolHapticWidget.h"
#include "ui_TelecontrolHapticWidget.h"
#include "TelecontrolGamepadWidget.h"
#include <math.h>

TelecontrolHapticWidget::TelecontrolHapticWidget(QString unit, const TelecontrolConfig::TcMove &method, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TelecontrolHapticWidget),
    mUnit(unit),
    mMethod(method.name)
{
    ui->setupUi(this);
    ui->groupBox->setTitle(method.name);

    // Setup sliders and their range
    ui->gainSlider->setRange(0, TelecontrolGamepadWidget::numSteps);
    ui->gainSlider->setValue(0);
    for(int i=TelecontrolGamepadWidget::numSteps; i>0; i--)
    {
        if( (pow(2.0, i) / pow(2.0, TelecontrolGamepadWidget::numSteps)) <= method.sensitivity)
        {
            ui->gainSlider->setValue(i);
            break;
        }
    }
    ui->forceSlider->setRange(0, TelecontrolGamepadWidget::numSteps);
    ui->forceSlider->setValue(0);
    for(int i=TelecontrolGamepadWidget::numSteps; i>0; i--)
    {
        if( (pow(2.0, i) / pow(2.0, TelecontrolGamepadWidget::numSteps)) <= method.forceScaling)
        {
            ui->forceSlider->setValue(i);
            break;
        }
    }

    // Add checkbox for each haptic axes
    for(int i=0; i<method.axeNames.size(); i++)
    {
        QString axisName = method.axeNames[i];
        QCheckBox* box = new QCheckBox(axisName);
        box->setChecked(method.inverts.value(i, false));
        mCheckboxes << box;
        connect(box, SIGNAL(toggled(bool)), SLOT(sendHapticUpdate()));
        ui->invertLayout->insertWidget(i, box);
    }
}

TelecontrolHapticWidget::~TelecontrolHapticWidget()
{
    delete ui;
}

void TelecontrolHapticWidget::sendHapticParamatersUpdate()
{
    double sensitivity = pow(2.0, ui->gainSlider->value()) / pow(2.0, TelecontrolGamepadWidget::numSteps);
    double forceScaling  = pow(2.0, ui->forceSlider->value()) / pow(2.0, TelecontrolGamepadWidget::numSteps);
    QList<bool> inverts;
    foreach(QCheckBox* box, mCheckboxes){
        inverts << box->isChecked();
    }
    emit updateHapticParameters(mUnit, mMethod, sensitivity, forceScaling, inverts);
}

void TelecontrolHapticWidget::on_gainSlider_sliderMoved(int /*position*/)
{
    sendHapticParamatersUpdate();
}

void TelecontrolHapticWidget::on_forceSlider_sliderMoved(int /*position*/)
{
    sendHapticParamatersUpdate();
}
