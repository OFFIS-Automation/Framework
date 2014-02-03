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

#include "DoubleEdit.h"

DoubleEdit::DoubleEdit(QWidget *parent) : StringEdit(parent)
{
    lineEdit->setValidator(new QDoubleValidator(this));
}

QString DoubleEdit::asString()
{
    return QString("%1").arg(mValue.toDouble());
}

QVariant DoubleEdit::editValue(bool& ok)
{
    return lineEdit->text().toDouble(&ok);
}
