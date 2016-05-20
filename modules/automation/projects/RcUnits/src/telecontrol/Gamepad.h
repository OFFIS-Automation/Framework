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

#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QThread>
#include <QMap>
#include <telecontrol/GamepadInterface.h>

class Gamepad : public QThread, public GamepadInterface
{
public:
    virtual QString getName() = 0;
    virtual int getResolution() const = 0;
    virtual GamepadType getGamepadType() const = 0;
    virtual void update(QMap<int, double>& joysticks, QMap<int, bool>& buttons) = 0;

    virtual ~Gamepad();
    void stop();

signals:
    void dataUpdated(const QMap<int,double>& data, const QString& gamepadName = QString());
    void buttonToggled(int buttonId, bool pressed = false, const QString& gamepadName = QString());

protected:
    Gamepad();
    bool mStop;

private:
    Q_OBJECT
};

#endif //GAMEPAD_H
