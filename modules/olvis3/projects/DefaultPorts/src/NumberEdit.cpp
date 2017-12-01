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

#include "NumberEdit.h"

#include <QDebug>

NumberEdit::NumberEdit(QWidget *parent) : AbstractPortEditWidget(parent)
{
    mSpinBox = new QSpinBox(this);
    mSpinBox->setObjectName("spinBox");
    ui->layout->insertWidget(0, mSpinBox);
}

NumberEdit::~NumberEdit()
{
}

void NumberEdit::setInfo(const PortInfo &info)
{
    int min = info.constraints.value("min", QVariant(INT_MAX)).toInt();
    int max = info.constraints.value("max", QVariant(INT_MAX)).toInt();
    if(min != mSpinBox->minimum() || max != mSpinBox->maximum()){
       AbstractPortEditWidget::setInfo(info);
       mSpinBox->setRange(min, max);
    }

    int stepping = info.constraints.value("div", QVariant(1)).toInt();
    if(stepping != mSpinBox->singleStep()){
        AbstractPortEditWidget::setInfo(info);
        mSpinBox->setSingleStep(qMax(1, stepping));
    }

    // Set tooltip
    mSpinBox->setToolTip(tr("Minimum: %1; Maximum: %2").arg(min).arg(max));
    if(stepping != mSpinBox->singleStep()){
        mSpinBox->setToolTip(tr("%1; Step: %2").arg(mSpinBox->toolTip()).arg(stepping));
    }
}

QString NumberEdit::asString()
{
    return QString::number(mValue.toInt());
}

void NumberEdit::onStartEdit()
{
    bool oldState = mSpinBox->blockSignals(true);
    mSpinBox->setValue(mValue.toInt());
    mSpinBox->blockSignals(oldState);
}

QVariant NumberEdit::editValue(bool&)
{
    return mSpinBox->value();
}
