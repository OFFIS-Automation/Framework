// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#include "DoubleEdit.h"

DoubleEdit::DoubleEdit(QWidget *parent) : AbstractPortEditWidget(parent)
{
    mSpinBox = new QDoubleSpinBox(this);
    mSpinBox->setObjectName("spinBox");

    ui->layout->insertWidget(0, mSpinBox);
}

DoubleEdit::~DoubleEdit()
{
}

void DoubleEdit::setInfo(const PortInfo &info)
{
    double min = info.constraints.value("min", QVariant(INT_MAX)).toDouble();
    double max = info.constraints.value("max", QVariant(INT_MAX)).toDouble();
    if(min != mSpinBox->minimum() || max != mSpinBox->maximum()){
       AbstractPortEditWidget::setInfo(info);
       mSpinBox->setRange(min, max);
    }

    int decimals = info.constraints.value("decimals", QVariant(2)).toInt();
    if(decimals != mSpinBox->decimals()){
        AbstractPortEditWidget::setInfo(info);
        mSpinBox->setDecimals(decimals);
    }
}

QString DoubleEdit::asString()
{
    return QString::number(mValue.toDouble());
}

void DoubleEdit::onStartEdit()
{
    bool oldState = mSpinBox->blockSignals(true);
    mSpinBox->setValue(mValue.toDouble());
    mSpinBox->blockSignals(oldState);
}

QVariant DoubleEdit::editValue(bool&)
{
    return mSpinBox->value();
}
