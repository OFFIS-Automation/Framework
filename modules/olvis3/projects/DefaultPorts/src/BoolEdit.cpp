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

#include "BoolEdit.h"

BoolEdit::BoolEdit(QWidget *parent):  AbstractPortEditWidget(parent)
{
    mYesButton = new QPushButton(tr("Yes"), this);
    mYesButton->setObjectName("yes");
    mYesButton->setCheckable(true);

    mNoButton = new QPushButton(tr("No"), this);
    mNoButton->setObjectName("no");
    mNoButton->setCheckable(true);

    ui->layout->insertWidget(0, mYesButton);
    ui->layout->insertWidget(1, mNoButton);

    QMetaObject::connectSlotsByName(this);
}

QString BoolEdit::asString()
{
    return mValue.toBool() ? tr("Yes") : tr("No");
}

BoolEdit::~BoolEdit()
{
}

void BoolEdit::onStartEdit()
{
    if(mValue.isValid()){
        mYesButton->setChecked(mValue.toBool());
        mNoButton->setChecked(!mValue.toBool());
    }
}

void BoolEdit::on_yes_clicked()
{
    onButtonClicked(true);
}

void BoolEdit::on_no_clicked()
{
    onButtonClicked(false);
}

void BoolEdit::onButtonClicked(bool yesButton)
{
    // Update GUI
    mYesButton->setChecked(yesButton);
    mNoButton->setChecked(!yesButton);

    // Update value, submit if necessary
    mValue = yesButton;
    if(mAutoSubmit){
        editFinished(yesButton);
    }
}
