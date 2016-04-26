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

#include "NumberEdit.h"

#include <QDebug>

NumberEdit::NumberEdit(QWidget *parent) : AbstractPortEditWidget(parent)
{
    mSpinBox = new QSpinBox(this);
    mSpinBox->setObjectName("spinBox");
    ui->layout->insertWidget(0, mSpinBox);

    QMetaObject::connectSlotsByName(this);
}

NumberEdit::~NumberEdit()
{
}

void NumberEdit::onStartEdit()
{
    int min = mInfo.constraints.value("min", QVariant(INT_MAX)).toInt();
    int max = mInfo.constraints.value("max", QVariant(INT_MAX)).toInt();
    mSpinBox->setRange(min, max);
    mSpinBox->setValue(mValue.toInt());

    int stepping = mInfo.constraints.value("div", QVariant(1)).toInt();
    mSpinBox->setSingleStep(qMax(1, stepping));
}

QString NumberEdit::asString()
{
    return QString::number(mValue.toInt());
}

QVariant NumberEdit::editValue(bool&)
{
    return mSpinBox->value();
}

void NumberEdit::on_spinBox_editingFinished()
{
    if(mSpinBox->hasFocus()){ // enter was pressed
        submit();
    }
}
