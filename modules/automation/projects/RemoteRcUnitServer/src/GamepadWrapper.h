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

#ifndef GAMEPADWRAPPER_H
#define GAMEPADWRAPPER_H

#include <QObject>
#include <QMap>
class GamepadWrapper : public QObject
{
    Q_OBJECT
public:
    explicit GamepadWrapper(QObject *parent = 0);
    void handleData(const QMap<int, double>& data) { emit dataUpdate(data); }
    void handleButton(int button, bool pressed) { emit buttonToggled(button, pressed); }
signals:
    void buttonToggled(int button, bool pressend);
    void dataUpdate(const QMap<int,double>& data);
public slots:

};

#endif // GAMEPADWRAPPER_H
