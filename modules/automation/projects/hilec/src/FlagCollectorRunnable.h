// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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


#ifndef FLAGCOLLECTORRUNNABLE_H
#define FLAGCOLLECTORRUNNABLE_H

#include <QRunnable>
#include <RcUnitBase.h>

class FlagCollectorRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit FlagCollectorRunnable(RcUnitBase *rcUnit);
    void setIdle() { mFinished = false; }
    bool finished() { return mFinished; }
signals:
    void flagsUpdated(const QString& unitName, const QVariantList& flags);
protected:
    void run();
    RcUnitBase* mRcUnit;
    bool mFinished;

};

#endif // FLAGCOLLECTORRUNNABLE_H
