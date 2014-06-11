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

#ifndef TRACER_H
#define TRACER_H

#include <QString>
#include <QMutex>
#include <QByteArray>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QMap>

#include <filter/FilterBaseGlobal.h>


class SubTracer;
class Timer;
class FILTERBASESHARED_EXPORT Tracer : public QThread
{
public:
    static Tracer& instance();
    virtual ~Tracer();
    void setEnabled(bool enabled);
    double timestamp();
    int labelId(const QString& name);
    SubTracer subTracer(int filterId);
    SubTracer subTracer(const QString& filterId);
    void trace(int event, int filter, int value, int id);

    enum Events
    {
        EmptyLabel = 0,
        ProcessorStarted,
        ProcessorActivated,
        ProcessorFinished,
        ProcessorStopped,
        FilterInitialized,
        FilterStarted,
        FilterActivated,
        PortFired,
        FilterFinished,
        FilterStopped,
        FilterDeinitialized,
        SystemStarted,
        SystemStopped,
        UserLabel = 100
    };

private:
    Tracer();
    void run();
    struct TraceElement
    {
        TraceElement(double t,int e,int n,int v, int i) : timestamp(t), event(e), name(n), value(v) ,id(i){}
        double timestamp;
        int event;
        int name;
        int value;
        int id;
    };
    QByteArray* mCurrentTrace;
    bool mCurrentTraceId;
    QByteArray mTraces[2];
    QMutex mMutex, mLabelMutex;
    Timer *mTimer;
    bool mStop;
    QFile mTransFile;
    QTextStream mTransStream;
    int mNextLabelId;
    QMap<QString, int> mLabels;
    bool mEnabled;
};

class SubTracer
{
public:
    SubTracer(int filterId, int id = 0);
    SubTracer(const QString& filterId, int id = 0);
    void setId(int id) { mId = id; }
    int id() const { return mId; }
    inline void trace(int event, int value = 0) { mTracer.trace(event, mFilterId, value, mId); }
private:
    Tracer& mTracer;
    int mId;
    int mFilterId;
};

#endif // TRACER_H
