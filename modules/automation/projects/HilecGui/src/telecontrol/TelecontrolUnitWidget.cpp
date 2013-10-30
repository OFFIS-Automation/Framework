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

#include "TelecontrolUnitWidget.h"
#include "ui_TelecontrolUnitWidget.h"
#include <QCheckBox>
#include <QDebug>
#include <cmath>

TelecontrolUnitWidget::TelecontrolUnitWidget(const QString& unit, const TelecontrolConfig::TcJostick &method, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TelecontrolUnitWidget),
    mUnit(unit),
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

TelecontrolUnitWidget::~TelecontrolUnitWidget()
{
    delete ui;
}

void TelecontrolUnitWidget::on_slider_sliderMoved(int position)
{
    sendUpdate(position);
}

void TelecontrolUnitWidget::sendUpdate(int sliderPosition)
{
    if(sliderPosition < 0)
        sliderPosition = ui->slider->value();
    double sensitivity = powf(2.0, sliderPosition) / powf(2.0, numSteps);
    QList<bool> inverts;
    foreach(QCheckBox* box, mCheckboxes)
        inverts << box->isChecked();
    emit updateTelecontrol(mUnit, mMethod, sensitivity, inverts);
}

void TelecontrolUnitWidget::changeSlider(const QString &unit, bool increase)
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
