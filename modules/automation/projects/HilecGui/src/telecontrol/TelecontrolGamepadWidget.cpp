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

#include "TelecontrolGamepadWidget.h"
#include "ui_TelecontrolGamepadWidget.h"
#include <QCheckBox>
#include <QDebug>
#include <cmath>

TelecontrolGamepadWidget::TelecontrolGamepadWidget(const QString& unitName, const TelecontrolConfig::TcMove &method, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TelecontrolGamepadWidget),
    mUnit(unitName),
    mMethod(method.name)
{
    ui->setupUi(this);
    ui->groupBox->setTitle(method.name);
    ui->slider->setRange(0, numSteps);
    ui->slider->setValue(0);
    for(int i=numSteps; i>0; i--)
    {
        if( (powf(2.0, i) / powf(2.0, numSteps)) <= method.sensitivity)
        {
            ui->slider->setValue(i);
            break;
        }
    }
    for(int i=0; i<method.axeNames.size(); i++)
    {
        QString axisName = method.axeNames[i];
        QCheckBox* box = new QCheckBox(axisName);
        box->setChecked(method.inverts.value(i, false));
        mCheckboxes << box;
        connect(box, SIGNAL(toggled(bool)), SLOT(sendUpdate()));
        ui->invertLayout->insertWidget(i, box);
    }
}

TelecontrolGamepadWidget::~TelecontrolGamepadWidget()
{
    delete ui;
}

void TelecontrolGamepadWidget::on_slider_sliderMoved(int position)
{
    sendUpdate(position);
}

void TelecontrolGamepadWidget::sendUpdate(int sliderPosition)
{
    if(sliderPosition < 0)
        sliderPosition = ui->slider->value();
    double sensitivity = powf(2.0, sliderPosition) / powf(2.0, numSteps);
    QList<bool> inverts;
    foreach(QCheckBox* box, mCheckboxes)
        inverts << box->isChecked();
    emit updateGamepadParameters(mUnit, mMethod, sensitivity, inverts);
}

void TelecontrolGamepadWidget::changeSlider(const QString &unit, bool increase)
{
    if(unit != mUnit)
        return;
    int current = ui->slider->value();
    if(increase)
        current++;
    else
        current--;
    ui->slider->setValue(current);
    sendUpdate(ui->slider->value());
}
