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

#include "StringEdit.h"

StringEdit::StringEdit(QWidget *parent) :
    AbstractPortEditWidget(parent)
{
    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName("lineEdit");
    ui->layout->insertWidget(0, lineEdit);
    QMetaObject::connectSlotsByName(this);
}

StringEdit::~StringEdit()
{
}

void StringEdit::onStartEdit()
{
    lineEdit->setText(asString());
    lineEdit->selectAll();
    lineEdit->setFocus();
}

QString StringEdit::asString()
{
    return mValue.toString();
}

void StringEdit::on_lineEdit_returnPressed()
{
    submit();
}

QVariant StringEdit::editValue(bool&)
{
    return lineEdit->text();
}
