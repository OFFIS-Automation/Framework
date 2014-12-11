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

#ifndef SENSORSYSTEM_TIMER_H
#define SENSORSYSTEM_TIMER_H

#include "SensorDataSystem.h"

class SENSORDATASYSTEMSHARED_EXPORT Timer
{
public:
    Timer();
    virtual ~Timer();
    void start();
    qint64 restart();
    qint64 elapsed() const;
    double elapsedSeconds() const;

private:
    struct TimerData;
    TimerData* d;
};

#endif // SENSORSYSTEM_TIMER_H

