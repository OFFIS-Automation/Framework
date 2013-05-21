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

#ifndef HAPTICINTERFACE_H
#define HAPTICINTERFACE_H

#include <QtGui/QVector3D>


struct HapticResponse
{
    QVector3D position;
    QVector3D forces;
};

class HapticEndpoint {
public:
    /**
     * @brief return the current position and force of the endpoint
     * The current absolute positions are normalized values between [0:1].
     * The forces are normalized values between [0:1]
     */
    virtual const HapticResponse currentHapticData() = 0;
    /**
     * @brief moves the endpoint to a new position and returns the reached position and the forces. tHe movement is normally between 1 and 10ms.
     * @param targetPositions absolute position normalized to [0:1]
     * @return @see currentData
     */
    virtual const HapticResponse hapticMovement(const QVector3D& targetPositions) = 0;
};

/**
 * @brief The HapticInterface class
 * When subclassing, the constructor should always work and raise no errors nor warnings
 * use the connectHardware method to check for the existance of the interface
 */
class HapticInterface {
public:
    /**
     * @brief connect to the controller hardware;
     * @throws std::exception if connection cannot be established
     */
    virtual void connectHardware(const QString& configfileName) = 0;

    /**
     * @brief disconnect from the hardware
     */
    virtual void releaseHardware() = 0;
    /**
     * @brief enables the haptic device to work on @a endpoint
     * @param endpoint the target endpint that can be controlled
     * After enable is called, the haptic interface can work on @a endpoint.
     */
    virtual void enable(HapticEndpoint* endpoint) = 0;
    /**
     * @brief disables the haptic interface
     * After disable was called, the reference endpoint given by @a enable becomes invalid.
     */
    virtual void disable() = 0;
    /**
     * @brief sets the movement scaling parameter
     * @param scaling value (0:1]
     * @param forceScaling value (0:1]
     * The scaling must be taken into account when working on the haptic endpoint
     * The scaling is always greater than zero and has a maximum of one
     * scaling should be applied to the movement. forceScaling to the feedback force
     */
    virtual void setScaling(double scaling, double forceScaling) = 0;

    virtual QWidget* createWidget() { return 0; }

};

Q_DECLARE_INTERFACE(HapticInterface, "com.amir.hilec.HapticInterface/1.0")

#endif // HAPTICINTERFACE_H
