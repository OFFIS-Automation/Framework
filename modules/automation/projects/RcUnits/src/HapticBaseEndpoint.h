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


#ifndef HAPTICBASEENDPOINT_H
#define HAPTICBASEENDPOINT_H

#include <telecontrol/HapticInterface.h>

class HapticBaseEndpoint : public HapticEndpoint
{
public:
    virtual void connectHapticDevice(QObject* hapticDevice) = 0;
    virtual void disconnectHapticDevice(QObject* hapticDevice) = 0;
    virtual void updateHapticSensitivity(const QString& unitName, double sensitivity, double forceScaling, const QList<bool>& inverts) = 0;
    virtual bool hasHapticControl() const = 0;
};

#endif // HAPTICBASEENDPOINT_H
