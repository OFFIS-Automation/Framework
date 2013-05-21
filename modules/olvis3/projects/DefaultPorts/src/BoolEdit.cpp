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

#include "BoolEdit.h"

BoolEdit::BoolEdit(QWidget *parent):  AbstractPortEditWidget(parent)
{
    yes = new QPushButton(tr("Yes"), this);
    yes->setObjectName("yes");
    no = new QPushButton(tr("No"), this);
    no->setObjectName("no");
    ui->layout->insertWidget(0, yes);
    ui->layout->insertWidget(1, no);
    ui->submit->setVisible(false); // no submit needed for this
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
    if(mValue.isValid())
    {
        if(mValue.toBool())
            yes->setFocus();
        else
            no->setFocus();
    }
    else
        ui->cancel->setFocus();
}

void BoolEdit::on_yes_clicked()
{
    editFinished(true);
}

void BoolEdit::on_no_clicked()
{
    editFinished(false);
}
