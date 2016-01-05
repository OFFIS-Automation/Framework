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
// along with this program.  If not, see <http:#www.gnu.org/licenses/>.

#include "DateTimeEdit.h"

#include <QDateTime>
#include <QDateTimeEdit>
#include <ports/DateTimePort.h>

DateTimeEdit::DateTimeEdit(QWidget *parent) :
    AbstractPortEditWidget(parent)
{
    edit = new QDateTimeEdit(mValue.toDateTime());
    edit->setDisplayFormat("yyyy/MM/dd hh:mm:zzz");
    ui->layout->insertWidget(0, edit);
}

QString DateTimeEdit::asString()
{
    return mValue.toDateTime().toString();
}

void DateTimeEdit::onStartEdit()
{
    QDateTime dt = mValue.toDateTime();
    if(!dt.isValid())
        dt = QDateTime::currentDateTime();
    edit->setDateTime(dt);
    edit->setFocus();
}


QVariant DateTimeEdit::editValue(bool& ok)
{
    QDateTime dateTime = edit->dateTime();
    ok = dateTime.isValid();
    return dateTime;
}
