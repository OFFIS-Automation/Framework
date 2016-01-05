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


#ifndef HWRCUNIT_H
#define HWRCUNIT_H

#include "UserRcUnit.h"
#include <QVariant>

/*
 * This is the base class for RC-Units that wrap an external hardware.
 * The inheriting class needs to implement the methods aquire and release
 */
class HwRcUnit : public UserRcUnit
{
public:
    virtual UserRcUnitType rcType() { return HwRcUnitType; }

public slots:
    /**
     * The acquire method connects the RcUnit to the real hardware
     * The methud MUST emit the hwConnectionStatusChanged signal with the connection status
     * after the acquire operation
     * If the acquire method was not successfull, it must throw an RcError
     */
    virtual void acquire() = 0;

    /**
     * The release method disconnects the RcUnit from the real hardware
     * The methud MUST emit the hwConnectionStatusChanged signal with the connection status
     * after the relelase operation
     * If the acquire method was not successfull, it must throw an RcError
     */
    virtual void release() = 0;

signals:
    /**
     * emit this signal every time the connection status is changed
     **/
    void hwConnectionStatusChanged(bool acquired);

protected:
    HwRcUnit() {}

private:
    Q_DISABLE_COPY(HwRcUnit)
    Q_OBJECT
};

#endif // HWRCUNIT_H
