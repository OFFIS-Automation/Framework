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


#ifndef USERRCUNIT_H
#define USERRCUNIT_H

#include <QObject>
#include <QVariantList>
#include "RcUnitTypes.h"


/*
 * This is the basic class for RcUnits.
*/
class UserRcUnit : public QObject
{
public:
    virtual ~UserRcUnit() {}
    virtual UserRcUnitType rcType() { return BaseRcUnitType; }

    /**
     * This method should return a definition of flags for this
     * RC-Unit. Flags are information that should be displayed to the user
     * For robots, this would be the current position or gripper status,
     * for sensors the current sensor value. If an empty rcFlagDefinitions
     * is returned, no flags are displayed for this unit.
     * If flag definitinos are returned, rcFlags is called periodically
     * (5-10Hz) to collect the flag data.
     */
    virtual RcFlagDefinitions rcFlagDefinitions() const {return RcFlagDefinitions();}

    /**
     * This method is called periodically if a flag definition was returned
     * by rcFlagDefinitions. The size of the list and the flag position must
     * match the definition.
     * Allowed types are int, double, string and boolean. Everything else
     * will be converted to string using the QVariant method.
     */
    virtual QVariantList rcFlags() { return QVariantList(); }

protected:
    UserRcUnit() {}

private:
    Q_DISABLE_COPY(UserRcUnit)
};


#endif // USERRCUNIT_H
