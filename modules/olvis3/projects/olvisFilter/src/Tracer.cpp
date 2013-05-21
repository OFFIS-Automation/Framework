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

#include <core/Tracer.h>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>

#include <Timer.h>

#include <stdio.h>

Tracer::Tracer() : mTimer(new Timer())
{
    mTraces[0].reserve(sizeof(TraceElement)*2000);
    mTraces[1].reserve(sizeof(TraceElement)*2000);
    mCurrentTrace = &mTraces[0];
    mCurrentTraceId = 0;
    mStop = false;

    mTransFile.setFileName(QCoreApplication::applicationDirPath() + "/trace.txt");
    mTransFile.open(QFile::WriteOnly | QFile::Text);
    mTransStream.setDevice(&mTransFile);
    mNextLabelId = UserLabel;
    mLabels[""] = EmptyLabel;
    mLabels["SystemStarted"] = SystemStarted;
    mLabels["SystemStopped"] = SystemStopped;
    mLabels["ProcessorStarted"] = ProcessorStarted;
    mLabels["ProcessorActivated"] = ProcessorActivated;
    mLabels["ProcessorFinished"] = ProcessorFinished;
    mLabels["ProcessorStopped"] = ProcessorStopped;
    mLabels["FilterInitialized"] = FilterInitialized;
    mLabels["FilterStarted"] = FilterStarted;
    mLabels["FilterActivated"] = FilterActivated;
    mLabels["PortFired"] = PortFired;
    mLabels["FilterFinished"] = FilterFinished;
    mLabels["FilterStopped"] = FilterStopped;
    mLabels["FilterDeinitialized"] = FilterDeinitialized;
    foreach(const QString& label, mLabels.keys())
        mTransStream << mLabels[label] << "=" << label << endl;
    start(IdlePriority);
}

Tracer::~Tracer()
{
    mStop = true;
    wait();
    delete mTimer;
}

int Tracer::labelId(const QString &label)
{
    QMutexLocker lock(&mLabelMutex);
    int id = mLabels.value(label, -1);
    if(id > -1)
        return id;
    id = mNextLabelId++;
    mLabels[label] = id;
    mTransStream << id << "=" << label << endl;
    mTransStream.flush();
    return id;
}

Tracer& Tracer::instance()
{
    static Tracer tracer;
    return tracer;
}

double Tracer::timestamp()
{
    return mTimer->elapsedSeconds();
}

void Tracer::trace(int event, int name, int value, int id)
{
    TraceElement te(timestamp(), event, name, value, id);
    mMutex.lock();
    void* data = &te;
    mCurrentTrace->append((char*)data, sizeof(TraceElement));
    mMutex.unlock();
}

void Tracer::run()
{
    QFile tracefile(QCoreApplication::applicationDirPath() + "/trace.bin");
    tracefile.open(QFile::WriteOnly);
    while(!mStop)
    {
        sleep(2);
        mMutex.lock();
        QByteArray* trace = mCurrentTrace;
        mCurrentTraceId = (mCurrentTraceId+1) % 2;
        mCurrentTrace = &mTraces[mCurrentTraceId];
        mMutex.unlock();
        tracefile.write(*trace);
        tracefile.flush();
        int size = trace->capacity();
        trace->resize(0);
        trace->reserve(size);
    }
}


SubTracer::SubTracer(int filterId, int id) : mTracer(Tracer::instance())
{
    mFilterId = filterId;
    mId = id;
}

SubTracer::SubTracer(const QString& filterName, int id) : mTracer(Tracer::instance())
{
    mFilterId = mTracer.labelId(filterName);
    mId = id;
}


