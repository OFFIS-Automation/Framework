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

#include "WindowsGamepad.h"

#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <telecontrol/TcConfig.h>
#include <QStringList>
#include <Notifications.h>

bool WindowsGamepad::initialize()
{
    setObjectName("Gamepad: " + getName());
    try{
        createMapping();

        /* Set the data format to "simple joystick" - a predefined data format
        *
        * A data format specifies which controls on a device we are interested in,
        * and how they should be reported. This tells DInput that we will be
        * passing a DIJOYSTATE structure to IDirectInputDevice::GetDeviceState().
        */
        if(FAILED(mDevice->SetDataFormat( &c_dfDIJoystick2 ))){
            throw std::runtime_error(qPrintable(tr("Could not set data format to joystick.")));
        }

        /* Enumerate the axes of the device to set the range of each axis and
         * make sure the axes are set to ABSOLUTE mode for 3DConnextion.
         * For 3DConnextion enumerate only axis.
         */
        if(FAILED(mDevice->EnumObjects( WindowsGamepad::enumObject, this, DIDFT_ALL))){
            throw std::runtime_error(qPrintable(tr("Could not enumerate objects")));
        }

        /* Aquire the device. Afterwards it is possible to get state information
         * from the device using e.g. GetDeviceState.
         */
        if(FAILED(mDevice->Acquire())){
            throw std::runtime_error(qPrintable(tr("Could not acquire device")));
        }

        return true;
    } catch(const std::exception& e) {
        qWarning() << tr("Could not initialize gamepad:") << " " <<  e.what();
        return false;
    }
}

void WindowsGamepad::createMapping()
{
    // Load the settings for the supported devices from an ini-file.
    QString settingsFile = QCoreApplication::applicationDirPath() + "/gamepads.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);
    foreach(QString group, settings.childGroups()){
        settings.beginGroup(group);
        QStringList guids = settings.value("guids").toStringList();
        if(guids.contains(mGuid)){
            // Type
            mGamepadType = settings.value("type").toString().compare("xbox", Qt::CaseInsensitive) == 0 ? XBoxGamepad : DefaultGamepad;
            // Mapping
            mButtonMapping[Tc::Gamepad::NorthButton] = settings.value("NorthButton").toInt();
            mButtonMapping[Tc::Gamepad::WestButton]  = settings.value("WestButton").toInt();
            mButtonMapping[Tc::Gamepad::EastButton]  = settings.value("EastButton").toInt();
            mButtonMapping[Tc::Gamepad::SouthButton] = settings.value("SouthButton").toInt();
            mButtonMapping[Tc::Gamepad::LeftShoulderUpperButton]  = settings.value("LeftShoulderUpperButton").toInt();
            mButtonMapping[Tc::Gamepad::LeftShoulderLowerButton]  = settings.value("LeftShoulderLowerButton").toInt();
            mButtonMapping[Tc::Gamepad::RightShoulderUpperButton] = settings.value("RightShoulderUpperButton").toInt();
            mButtonMapping[Tc::Gamepad::RightShoulderLowerButton] = settings.value("RightShoulderLowerButton").toInt();
            mSwitchZJoysticks = settings.value("SwitchZJoysticks", false).toBool();

            return;
        }
        settings.endGroup();
    }
    qWarning() << "No gamepad mapping found for" << mName << "guid:" << mGuid;
    qWarning() << "Add an entry with its guid into the mapping file <gamepads.ini> in the applications root directory";
    throw std::runtime_error("Gamepad mapping not found!");
}

void WindowsGamepad::assignButton(QMap<int, bool>& buttons, BYTE* data, int buttonId)
{
    int readId = mButtonMapping.value(buttonId, 0) - 1;
    if(readId < 0){
        return;
    }
    buttons[buttonId] = data[readId] != 0;
}

void WindowsGamepad::update(QMap<int, double> &joysticks, QMap<int, bool> &buttons)
{
    DIJOYSTATE2& status = mState;

    // Axes
    joysticks[Tc::Gamepad::LeftJoystickX] = correctedValue(float(status.lX));
    joysticks[Tc::Gamepad::LeftJoystickY] = correctedValue(-float(status.lY));
    if(mGamepadType == XBoxGamepad){
        joysticks[Tc::Gamepad::RightJoystickX] = correctedValue(float(status.lRx));
        joysticks[Tc::Gamepad::RightJoystickY] = correctedValue(-float(status.lRy));
        joysticks[Tc::Gamepad::TriggerJoystick] = correctedValue(-float(status.lZ));
    } else if(mSwitchZJoysticks){
        joysticks[Tc::Gamepad::RightJoystickX] = correctedValue(float(status.lRz));
        joysticks[Tc::Gamepad::RightJoystickY] = correctedValue(-float(status.lZ));
    } else {
        joysticks[Tc::Gamepad::RightJoystickX] = correctedValue(float(status.lZ));
        joysticks[Tc::Gamepad::RightJoystickY] = correctedValue(-float(status.lRz));
    }

    // Buttons
    for(int i=Tc::Gamepad::NorthButton; i<= Tc::Gamepad::RightShoulderLowerButton; i++){
        assignButton(buttons, status.rgbButtons, i);
    }
    if(mGamepadType == XBoxGamepad){
        buttons[Tc::Gamepad::LeftShoulderLowerButton] = float(status.lZ) > 0.2;
        buttons[Tc::Gamepad::RightShoulderLowerButton] = float(status.lZ) < -0.2;
        buttons[Tc::Gamepad::TriggerButton] = buttons[Tc::Gamepad::LeftShoulderLowerButton] || buttons[Tc::Gamepad::RightShoulderLowerButton];
    }


    bool up = false, down = false, left = false, right = false;
    switch(status.rgdwPOV[0]) {
        // contour-clockwise from left direction
        case 9000:  right = true; break;
        case 4500:  right = true; up = true; break;
        case 0:     up = true; break;
        case 31500: left = true; up = true; break;
        case 27000: left = true; break;
        case 22500: left = true; down = true; break;
        case 18000: down = true; break;
        case 13500: right = true; down = true; break;
    }
    buttons[Tc::Gamepad::ButtonUp] = up;
    buttons[Tc::Gamepad::ButtonDown] = down;
    buttons[Tc::Gamepad::ButtonLeft] = left;
    buttons[Tc::Gamepad::ButtonRight] = right;
    buttons[Tc::Gamepad::ImplicitActivationButton] = false;
    foreach(double value, joysticks.values()){
        if(value != 0.0){
            buttons[Tc::Gamepad::ImplicitActivationButton] = true;
            break;
        }
    }
}

float WindowsGamepad::correctedValue(float v)
{
    v /= (float)getResolution();
    if(v >-0.1 && v < 0.1){
        return 0.0;
    }
    return v;
}

WindowsGamepad::WindowsGamepad(const QString &name, const QString &guid) :
    mName(name),
    mGuid(guid)
{
    mDevice = 0;
}

int WindowsGamepad::getResolution() const
{
    return 64;
}

WindowsGamepad::~WindowsGamepad()
{
    stop();
    wait();
    if(mDevice != 0){
        // Release the event
        mDevice->SetEventNotification(NULL);
        mDevice->Unacquire();

        mDevice->Release();
        mDevice = NULL;
    }
}

void WindowsGamepad::run()
{
    mStop = false;
    QMap<int,double> joysticks;
    QMap<int,bool> buttons, lastButtons;

    while(!mStop){
        QElapsedTimer timer;
        timer.start();

        // Get the current device state
        if(FAILED(mDevice->GetDeviceState(sizeof(DIJOYSTATE2), &mState))){
            continue;
        }
        // Assign to internal data structures
        buttons.clear();
        update(joysticks, buttons);

        // Check for button toggle
        QMapIterator<int,bool> iter(buttons);
        while(iter.hasNext()){
            iter.next();
            int buttonId = iter.key();
            bool value = iter.value();

            if(!lastButtons.contains(buttonId) || (lastButtons.contains(buttonId) && lastButtons[buttonId] != value)){
                if(lastButtons.contains(buttonId)){
                    emit buttonToggled(buttonId, value, getName());
                } else {
                    if(value){
                        emit buttonToggled(buttonId, value, getName());
                    }
                }
            }
        }
        lastButtons = buttons;

        // Handle joysticks data
        emit dataUpdated(joysticks, getName());

        // Eventually sleep thread, limit update rate (20Hz)
        int remaining = 50 - timer.elapsed();
        if(remaining > 0){
            msleep(remaining);
        }
    }
}

BOOL CALLBACK WindowsGamepad::enumObject(const DIDEVICEOBJECTINSTANCE* inst, VOID* pContext)
{
    WindowsGamepad* gamepad = (WindowsGamepad*) pContext;

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if(inst->dwType & DIDFT_AXIS){
        // Set the range for the axis
        DIPROPRANGE diprg;
        diprg.diph.dwSize       = sizeof(DIPROPRANGE);
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        diprg.diph.dwHow        = DIPH_BYID;
        diprg.diph.dwObj        = inst->dwType; // Specify the enumerated axis
        diprg.lMin              = -gamepad->getResolution();
        diprg.lMax              = +(gamepad->getResolution()-1);

        if(FAILED(gamepad->mDevice->SetProperty(DIPROP_RANGE, &diprg.diph))){
            return DIENUM_STOP;
        }
    }

    return DIENUM_CONTINUE;
}
