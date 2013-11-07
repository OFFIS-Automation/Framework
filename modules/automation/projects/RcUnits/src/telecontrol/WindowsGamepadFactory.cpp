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

#include "WindowsGamepadFactory.h"

#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <telecontrol/GamepadInterface.h>

#include <QUuid>

Gamepad* WindowsGamepadFactory::sGamepad;
LPDIRECTINPUT8 WindowsGamepadFactory::sDirectInput;

WindowsGamepadFactory::WindowsGamepadFactory()
{
	sDirectInput = 0;
    sGamepad = 0;
    DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&sDirectInput, NULL );
}

WindowsGamepadFactory::~WindowsGamepadFactory()
{
    sDirectInput->Release();
}

WindowsGamepadFactory &WindowsGamepadFactory::instance()
{
    static WindowsGamepadFactory factory;
    return factory;
}

Gamepad* WindowsGamepadFactory::createGamepad()
{
	// force constructor/destructor call to acquire/release sDirectInput
    instance();
    if(sGamepad) delete sGamepad;
	sGamepad = 0;
	HRESULT hr;
	
	hr = sDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL, 
                WindowsGamepadFactory::enumDevices, 0 , DIEDFL_ATTACHEDONLY );
    return sGamepad;
}

BOOL CALLBACK WindowsGamepadFactory::enumDevices(const DIDEVICEINSTANCE *inst, void *)
{
    try
    {
        QString name = QString::fromWCharArray(inst->tszInstanceName);
        QString product = QString::fromWCharArray(inst->tszProductName);
        WindowsGamepad* gamepad;
        QString guid = QUuid(inst->guidProduct).toString().replace('{',"").replace('}',"");
        // Check for XBOX Gamepad (with special button assignment)
        if(name.compare(QString("Controller (XBOX 360 For Windows)")) == 0)
            gamepad = new WindowsXBOXGamepad(name);
        else
            gamepad = new WindowsGamepad(name, guid);

        if(!gamepad)
            throw std::runtime_error(qPrintable(tr("No wrapper for device: %1").arg(name)));

        HRESULT hr = sDirectInput->CreateDevice( inst->guidInstance, &gamepad->mDevice, NULL );
        if(FAILED(hr) || !gamepad->initialize())
        {
            delete gamepad;
            sDirectInput->Release();
            throw std::runtime_error(qPrintable(tr("Error initializing gamepad: %1").arg(name)));
        }
        else
            sGamepad = gamepad;
    }
    catch(const std::exception& e)
    {
        qWarning() << tr("Could not create device:") << " " << e.what();
            return DIENUM_CONTINUE;
    }
    return DIENUM_STOP;
}
