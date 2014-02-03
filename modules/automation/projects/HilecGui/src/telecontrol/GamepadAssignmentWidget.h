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

#ifndef GAMEPADASSIGNMENTWIDGET_H
#define GAMEPADASSIGNMENTWIDGET_H

#include <QWidget>
#include <QLabel>
#include "telecontrol/TcConfig.h"

namespace Ui {
class GamepadAssignmentWidget;
}

class GamepadAssignmentWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GamepadAssignmentWidget(const QString &unit, QWidget *parent = 0);
    ~GamepadAssignmentWidget();
    
private:
    Ui::GamepadAssignmentWidget *ui;
    QString mUnit;

protected:
    QString labelNameForJoystick(Tc::Joystick joystick);

};

#endif // GAMEPADASSIGNMENTWIDGET_H
