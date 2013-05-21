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

#include "MinimumFps.h"
#include <QDebug>

REGISTER_FILTER(MinimumFps);

MinimumFps::MinimumFps()
{
    setName("MinimumFps");
    setGroup("helper");
    setDesc("Insert a variable delay in your chain. This end-to-end time of this processor in always >= the requred frames per second");
    mFps.setName("inFps");
    mFps.setDesc("minimump fps");
    mFps.setRange(0.1,1000.0);
    mFps.setDefault(5.0);
    addInputPort(mFps);
    mMode.setName("Mode");
    mMode.setDesc("Active wait is cpu-time consuming, but on most systems more precise than sleep wait");
    mMode.addChoice(SleepMode, "Sleep wait");
    mMode.addChoice(ActiveWaitMode, "Active wait");
    mMode.setDefault(SleepMode);
    addInputPort(mMode);
}

void MinimumFps::start()
{
    mTimer.start();
}

void MinimumFps::execute()
{
    double fps = mFps;
    double duration = (1000/fps);
    double elapsed = mTimer.elapsed();
    if(elapsed < duration)
    {
        if(mMode.getValue() == SleepMode)
            msleep(duration-elapsed);
        else
            while(mTimer.elapsed() < duration);
    }
    mTimer.restart();
}
