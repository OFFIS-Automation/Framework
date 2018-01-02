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

#ifndef HAPTICDEVICEINTERFACE_H
#define HAPTICDEVICEINTERFACE_H

#include <QMap>
#include <QThread>
#include <QMap>
#include <QObject>

/**
 * @brief The HapticEndpoint class
 * When subclassing, the provided methods should allow for integrating haptic
 * support into a remote controlled unit (endpoint).
 */
class HapticEndpoint {
public:
    virtual void connectHapticDevice(QObject* hapticDevice) = 0;
    virtual void disconnectHapticDevice(QObject* hapticDevice) = 0;
    virtual void updateHapticParameters(const QString& methodName, double sensitivity, double forceScaling, const QList<bool>& inverts) = 0;
    virtual bool hasHapticControl() const = 0;
};

/**
 * @brief The HapticDevice class
 * The haptic interface should return at least one instace of this claas
 */
class HapticDevice : public QThread {
    Q_OBJECT
public:
    /**
     * @brief Name for the haptic device
     * This method should return a unique name for the device
     * and thus should contain e.g. "vendor type (serial)".
     * @return name of the interface
     */
    virtual QString name() = 0;

    /**
     * @brief Enables the haptic device
     * Following to this method the haptic device should send updates using
     * the positionUpdate signal.
     */
    virtual void enable() = 0;

    /**
     * @brief Disables the haptic device
     */
    virtual void disable() = 0;

    /**
     * @brief Should create a widget for the device of appropriate
     * @return a widget for the haptic interface. The Widget may provide
     * additional information and settings for the device.
     */
    virtual QWidget* createWidget() { return 0; }

public slots:
    /**
     * @brief Handle given force data
     * @param data Force data which should be applied to the given axis. Each double value is in range [-1,1].
     */
    virtual void handleForceUpdate(const QMap<int,double>& data) = 0;

signals:
    /**
     * @brief Send position update
     * @param data Position data for the axis on the haptic device. Double value should be in range [-1,1].
     */
    void positionUpdated(const QMap<int,double>& data);

    /**
     * @brief Send update about a toggled button
     * @param buttonId
     * @param pressed
     */
    void buttonToggled(int buttonId, bool pressed);
};

class HapticInterface : public QObject {
public:
    virtual QList<HapticDevice *> availableDevices() = 0;
};

#define HapticInterface_iid "com.amir.hilec.HapticInterface/2.0"
Q_DECLARE_INTERFACE(HapticInterface, HapticInterface_iid)

#endif // HAPTICDEVICEINTERFACE_H
