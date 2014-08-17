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


#ifndef ROBOTRCUNIT_H
#define ROBOTRCUNIT_H

#include "HwRcUnit.h"
#include <QVariant>

class RobotRcUnit : public HwRcUnit
{
public:
    virtual UserRcUnitType rcType() { return RobotRcUnitType; }

    /**
     * This operation should stop the current movement.
     * It is called when a automation error occours or the stop button is pressed
     * by the user. If a movement method was active, it should be ended with an RcError
     */
    virtual void stop() = 0;

    /**
     * This method is used for storing user defined position.
     * A user may move the robot to a specific position and store the position
     * with an assigned name.
     * @return A storable QVariant. All basic types and Qt types (QVector3D, QPointF etc.)
     * are allowed as well as QVariantLists of those types (e.g. QVariantList with several double values)
     * If the current position cannot be sotred (e.g. sensors not initialized)
     * return an invalid QVariant
     */
    virtual QVariant rcGetPosition() = 0;

    /**
     * This method is used for restoring user defined position.
     * A user may move the robot to a specific position and store the position
     * with an assigned name. Later, the robot should return to this position.
     * @param pos The position as QVariant, as returned by rcGetPosition
     */
    virtual void rcSetPosition(const QVariant& pos) = 0;

protected:
    RobotRcUnit() {}

private:
    Q_DISABLE_COPY(RobotRcUnit)

};

#endif // ROBOTRCUNIT_H
