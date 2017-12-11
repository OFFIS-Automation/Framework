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

#include "RectEdit.h"

RectEdit::RectEdit(QWidget *parent) : StringEdit(parent)
{
    QRegExp exp("-?\\d*,-?\\d*,-?\\d*,-?\\d*");
    lineEdit->setValidator(new QRegExpValidator(exp, this));
}

QString RectEdit::asString()
{
    return rectToString(mValue.toRect());
}

QVariant RectEdit::editValue(bool& ok)
{
    return stringToRect(lineEdit->text(), ok);
}

QString RectEdit::rectToString(const QRectF &r)
{
    return QString("%1,%2,%3,%4").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QRectF RectEdit::stringToRect(const QString &s, bool &ok)
{
    ok = true;
    QStringList parts = s.split(",");
    if(parts.size() != 4)
    {
        ok = false;
        return QRect();
    }
    return QRectF(parts[0].toInt(), parts[1].toInt(), parts[2].toInt(), parts[3].toInt());
}
