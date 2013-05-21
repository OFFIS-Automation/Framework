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

#ifndef CUSTOMLOLEC_H
#define CUSTOMLOLEC_H

#include <QObject>

class CustomLolec : public QObject
{
    Q_OBJECT
public:
    CustomLolec();
public slots:
    void acquireHardware();
    void releaseHardware();
    void moveGamepad(double xAxis, double yAxis);

signals:
    void connectStatusChanged(bool connected);
};

#endif // CUSTOMLOLEC_H
