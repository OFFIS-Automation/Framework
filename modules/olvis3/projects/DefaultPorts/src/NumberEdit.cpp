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

#include "NumberEdit.h"

NumberEdit::NumberEdit(QWidget *parent) :
    AbstractPortEditWidget(parent)
{
    spinBox = new QSpinBox(this);
    spinBox->setObjectName("spinBox");
    ui->layout->insertWidget(0,spinBox);
}

NumberEdit::~NumberEdit()
{
}

void NumberEdit::onStartEdit()
{
    int min = mInfo.constraints.value("min", QVariant(INT_MAX)).toInt();
    int max = mInfo.constraints.value("max", QVariant(INT_MAX)).toInt();
    spinBox->setRange(min, max);
    spinBox->setValue(mValue.toInt());
    int stepping = mInfo.constraints.value("div", QVariant(1)).toInt();
    spinBox->setSingleStep(qMax(1, stepping));
    spinBox->setFocus();
}

QString NumberEdit::asString()
{
    return QString::number(mValue.toInt());
}

void NumberEdit::on_spinBox_editingFinished()
{
    if(spinBox->hasFocus()) // enter was pressed
        submit();
}

QVariant NumberEdit::editValue(bool&)
{
    return spinBox->value();
}
