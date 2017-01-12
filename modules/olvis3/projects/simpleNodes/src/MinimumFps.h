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

#ifndef MINIMUMPFS_H
#define MINIMUMPFS_H

#include <QThread>
#include <QElapsedTimer>

#include <filter/PluginInterface.h>
#include "Timer.h"
#include <ports/RealPort.h>
#include <ports/IntegerPort.h>

#include <QMutex>
#include <QWaitCondition>

class MinimumFps : public UserFilter, QThread
{
public:
    MinimumFps();
    virtual void start();
    virtual void execute();


protected:
    enum {
        SleepMode,
        ActiveWaitMode
    };
    in::Real mFps;
    in::Integer mMode;
    Timer mTimer;
};

#endif // MINIMUMPFS_H
