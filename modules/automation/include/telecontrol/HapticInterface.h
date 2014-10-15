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

#ifndef HAPTICDEVICEINTERFACE_H
#define HAPTICDEVICEINTERFACE_H

#include <QMap>
#include <QThread>

/**
 * @brief The HapticEndpoint class
 * When subclassing, the provided methods should allow for integrating haptic
 * support into a remote controlled unit (endpoint).
 */
class HapticEndpoint {
public:
    virtual void connectHapticDevice(QObject* hapticDevice) = 0;
    virtual void disconnectHapticDevice(QObject* hapticDevice) = 0;
    virtual void updateHapticSensitivity(const QString& unitName, double sensitivity, double forceScaling, const QList<bool>& inverts) = 0;
    virtual bool hasHapticControl() const = 0;
};

/**
 * @brief The HapticDevice class
 * When subclassing, the constructor should always work and raise no errors nor warnings
 * use the connectHardware method to check for the existance of the interface
 */
class HapticInterface : public QThread {
public:
    /**
     * @brief Connect to the haptic hardware.
     * @throws std::exception if connection cannot be established.
     */
    virtual void connectHardware(const QString& configfileName) = 0;

    /**
     * @brief Disconnect from the haptic hardware.
     */
    virtual void releaseHardware() = 0;

    /**
     * @brief Enables the haptic device
     * Follong this method the haptic device should send updates using
     * the positionUpdate signal.
     */
    virtual void enable() = 0;

    /**
     * @brief Disables the haptic device
     */
    virtual void disable() = 0;

    virtual QWidget* createWidget() { return 0; }

public slots:
    /**
     * @brief handleForceData
     * @param data Force data which should be applied to the given axis. Each double value is in range [-1,1].
     */
    virtual void handleForceData(const QMap<int,double>& data) = 0;

signals:
    /**
     * @brief Send position update.
     * @param data Position data for the axis on the haptic device. Double value should be in range [-1,1].
     */
    void positionUpdate(const QMap<int,double>& data);

    /**
     * @brief Send update about a toogled button.
     * @param buttonId
     * @param pressed
     */
    void buttonToggled(int buttonId, bool pressed);
};

#define HapticInterface_iid "com.amir.hilec.HapticInterface/2.0"
Q_DECLARE_INTERFACE(HapticInterface, HapticInterface_iid)

#endif // HAPTICDEVICEINTERFACE_H
