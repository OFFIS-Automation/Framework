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

#include "PointEdit.h"

PointEdit::PointEdit(QWidget *parent) : StringEdit(parent)
{
    QRegExp exp("\\d*,\\d*");
    lineEdit->setValidator(new QRegExpValidator(exp, this));
}

QString PointEdit::asString()
{
    return PointToString(mValue.toPoint());
}

QVariant PointEdit::editValue(bool& ok)
{
    return stringToPoint(lineEdit->text(), ok);
}

QString PointEdit::PointToString(const QPoint &p)
{
    return QString("%1,%2").arg(p.x()).arg(p.y());
}

QPointF PointEdit::stringToPoint(const QString &s, bool &ok)
{
    ok = true;
    QStringList parts = s.split(",");
    if(parts.size() != 2)
    {
        ok = false;
        return QPoint();
    }
    return QPointF(parts[0].toInt(), parts[1].toInt());
}
