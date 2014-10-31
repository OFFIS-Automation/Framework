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

#include "WindowsGamepad.h"

#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <telecontrol/TcConfig.h>
#include <QStringList>

bool WindowsGamepad::initialize()
{
    setObjectName("Gamepad: " + getName());
    try{
        createMapping();

        if(FAILED(mDevice->SetDataFormat( &c_dfDIJoystick2 )))
            throw std::runtime_error(tr("Could not set data format to joystick.").toStdString());

        if(FAILED(mDevice->EnumObjects( WindowsGamepad::enumObject, this, DIDFT_ALL )))
            throw std::runtime_error(tr("Could not enumerate objects").toStdString());

        if(FAILED(mDevice->Acquire()))
            throw std::runtime_error(tr("Could not acquire device").toStdString());
        return true;
    } catch(const std::exception& e) {
        qWarning() << tr("Could not initialize gamepad:") << " " <<  e.what();
        return false;
    }
}

void WindowsGamepad::createMapping()
{
    QString settingsFile = QCoreApplication::applicationDirPath() + "/gamepads.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);
    foreach(QString group, settings.childGroups())
    {
        settings.beginGroup(group);
        QStringList guids = settings.value("guids").toStringList();
        if(guids.contains(mGuid))
        {
            mButtonMapping[Tc::NorthButton] = settings.value("NorthButton").toInt();
            mButtonMapping[Tc::WestButton]  = settings.value("WestButton").toInt();
            mButtonMapping[Tc::EastButton]  = settings.value("EastButton").toInt();
            mButtonMapping[Tc::SouthButton] = settings.value("SouthButton").toInt();
            mButtonMapping[Tc::LeftShoulderUpperButton]  = settings.value("LeftShoulderUpperButton").toInt();
            mButtonMapping[Tc::LeftShoulderLowerButton]  = settings.value("LeftShoulderLowerButton").toInt();
            mButtonMapping[Tc::RightShoulderUpperButton] = settings.value("RightShoulderUpperButton").toInt();
            mButtonMapping[Tc::RightShoulderLowerButton] = settings.value("RightShoulderLowerButton").toInt();
            mSwitchZJoysticks = settings.value("SwitchZJoysticks", false).toBool();

            if(settings.value("type").toString().toLower() == "xbox"){
                mGamepadType = XBoxGamepad;
            } else {
                mGamepadType = DefaultGamepad;
            }
            return;
        }
        settings.endGroup();
    }
    qWarning() << "No gamepad mapping found for" << mName << "guid:" << mGuid;
    qWarning() << "Add an entry with its guid into the mapping file <gamepads.ini> in the root directory";
    throw std::runtime_error("Gamepad mapping not found!");
}

void WindowsGamepad::assingButton(QMap<int, bool>& buttons, BYTE* data, int buttonId)
{
    int readId = mButtonMapping.value(buttonId, 0) -1;
    if(readId < 0)
        return;
    buttons[buttonId] = data[readId] != 0;
}

void WindowsGamepad::update(QMap<int, double> &joysticks, QMap<int, bool> &buttons)
{
    DIJOYSTATE2& status = mState;
    joysticks[Tc::LeftJoystickX] = correctedValue(float(status.lX));
    joysticks[Tc::LeftJoystickY] = correctedValue(-float(status.lY));
    if(mGamepadType == XBoxGamepad)
    {
        joysticks[Tc::RightJoystickX] = correctedValue(float(status.lRx));
        joysticks[Tc::RightJoystickY] = correctedValue(-float(status.lRy));
    }
    else if(mSwitchZJoysticks)
    {
        joysticks[Tc::RightJoystickX] = correctedValue(float(status.lRz));
        joysticks[Tc::RightJoystickY] = correctedValue(-float(status.lZ));
    }
    else
    {
        joysticks[Tc::RightJoystickX] = correctedValue(float(status.lZ));
        joysticks[Tc::RightJoystickY] = correctedValue(-float(status.lRz));
    }

    for(int i=Tc::NorthButton; i<= Tc::RightShoulderLowerButton; i++)
        assingButton(buttons, status.rgbButtons, i);

    if(mGamepadType == XBoxGamepad)
    {
        buttons[Tc::LeftShoulderLowerButton] = float(status.lZ) > 0.8;
        buttons[Tc::RightShoulderLowerButton] = float(status.lZ) < -0.8;
    }
    bool up = false, down = false, left = false, right = false;
    switch(status.rgdwPOV[0]) // contour-clockwise from left direction
    {
        case 9000:  right = true; break;
        case 4500:  right =  true; up =  true; break;
        case 0:     up =  true; break;
        case 31500: left = true; up =  true; break;
        case 27000: left = true; break;
        case 22500: left = true; down = true; break;
        case 18000: down = true; break;
        case 13500: right = true; down = true; break;
    }
    buttons[Tc::ButtonUp] = up;
    buttons[Tc::ButtonDown] = down;
    buttons[Tc::ButtonLeft] = left;
    buttons[Tc::ButtonRight] = right;
}

float WindowsGamepad::correctedValue(float v)
{
    v /= (float)getResolution();
    if(v >-0.1 && v < 0.1)
        return 0.0;
    else
        return v;
}

WindowsGamepad::WindowsGamepad(const QString &name, const QString &guid)
    : mName(name),
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
    if(mDevice != 0)
    {
        mDevice->Release();
    }

}

void WindowsGamepad::run()
{
    mStop = false;
    QMap<int,double> joysticks;
    QMap<int,bool> buttons, lastButtons;
    while(!mStop)
    {
        QElapsedTimer timer;
        timer.start();
        if(FAILED(mDevice->GetDeviceState(sizeof(DIJOYSTATE2), &mState)))
            continue;
        update(joysticks, buttons);
        QMapIterator<int,bool> iter(buttons);
        while(iter.hasNext())
        {
            iter.next();
            int buttonId = iter.key();
            bool value = iter.value();
            if(lastButtons.contains(buttonId))
            {
                if(lastButtons[buttonId] != value) // value has changed
                emit buttonToggled(buttonId, value);
            }
            else if(value) // first run, emit if pressed
                emit buttonToggled(buttonId, value);
        }
        lastButtons = buttons;
        emit dataUpdate(joysticks);
        int remaining = 50 - timer.elapsed();
        if(remaining > 0)
            msleep(remaining);
    }
}

BOOL CALLBACK WindowsGamepad::enumObject(const DIDEVICEOBJECTINSTANCE* inst, VOID* pContext)
{
	WindowsGamepad* gamepad = (WindowsGamepad*) pContext;


    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( inst->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = inst->dwType; // Specify the enumerated axis
        diprg.lMin              = -gamepad->getResolution();
        diprg.lMax              = +gamepad->getResolution();
    
        // Set the range for the axis
        if( FAILED( gamepad->mDevice->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
            return DIENUM_STOP;
         
    }
    return DIENUM_CONTINUE;
}
