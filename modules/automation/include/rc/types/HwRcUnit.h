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


#ifndef HWRCUNIT_H
#define HWRCUNIT_H

#include "UserRcUnit.h"

/*
 * This is the base class for RC-Units that wrap an external hardware.
 * The inheriting class needs to implement the methods aquire and release
 */
class HwRcUnit : public UserRcUnit
{
    Q_OBJECT
public:
    virtual bool acquire() = 0;
    virtual void release() = 0;
public slots:
    void acquireHardware() { emit systemStatusChanged(acquire()); }
    void releaseHardware() { emit systemStatusChanged(false); }
signals:
    void systemStatusChanged(bool acquired);
protected:
    HwRcUnit() {}
    virtual ~HwRcUnit() {}

};

#endif // HWRCUNIT_H
