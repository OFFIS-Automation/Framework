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

#ifndef LOLECTOOLS_GAMEPAD_HQT
#define LOLECTOOLS_GAMEPAD_HQT

#include <QThread>
#include <QMap>

namespace Tc
{
    enum WheelButtons {
        ButtonUp = 1024,
        ButtonDown,
        ButtonLeft,
        ButtonRight
    };
}

class Gamepad : public QThread
{
public:
    virtual QString getName() = 0;
    virtual ~Gamepad();
    void stop();
signals:
    void dataUpdate(const QMap<int,double>& data);
    void buttonToggled(int id, bool pressed);

protected:
    Gamepad();
    bool mStop;
private:
    Q_OBJECT
};

#endif //LOLECTOOLS_GAMEPAD_HQT
