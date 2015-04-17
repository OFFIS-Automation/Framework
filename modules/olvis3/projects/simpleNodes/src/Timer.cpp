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

#include "Timer.h"
#include <QString>
#include "qglobal.h"

#ifdef Q_OS_WIN
#include <Windows.h>
struct Timer::TimerData
{
    void start()
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&startPos);
        qint64 tmp;
        QueryPerformanceFrequency((LARGE_INTEGER*)&tmp);
        freq = tmp;
    }

    double restart()
    {
        qint64 tmp;
        QueryPerformanceCounter((LARGE_INTEGER*)&tmp);
        double val = ((tmp-startPos)*1000)/freq;
        startPos = tmp;
        return val;
    }

    double elapsed() const
    {
        qint64 tmp;
        QueryPerformanceCounter((LARGE_INTEGER*)&tmp);
        return ((tmp-startPos)*1000)/freq;
    }
    qint64 startPos;
    double freq;
};

#else
#include <QElapsedTimer>
struct Timer::TimerData : public QElapsedTimer
{
};
#endif

Timer::Timer() : d(new TimerData())
{
    start();
}

Timer::~Timer()
{
    delete d;
}

qint64 Timer::restart()
{
    return d->restart();
}

qint64 Timer::elapsed() const
{
    return d->elapsed();
}

double Timer::elapsedSeconds() const
{
    return d->elapsed() / 1000.0;
}

void Timer::start()
{
    d->start();
}

