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

#ifndef PYTHONTYPECONVERTER_H
#define PYTHONTYPECONVERTER_H

#include <lolecs/Pose2d.h>

#include <QVariant>


struct _object;
class PythonTypeConverter
{
public:
    static QVariant toVariant(_object* input);
    static _object* fromVariant(const QVariant& var);
    static QString toString(_object* input, bool force = false);
    static _object* fromString(QString str);
protected:
    static QVariant bestFittingList(const QList<QVariant>& list);
    PythonTypeConverter();
    Q_DISABLE_COPY(PythonTypeConverter)
};

#endif // PYTHONTYPECONVERTER_H
