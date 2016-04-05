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

#include "WindowsTelecontrolFactory.h"

#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <telecontrol/GamepadInterface.h>
#include <QUuid>

QMap<QString, Gamepad *> WindowsTelecontrolFactory::sGamepadDevices;
LPDIRECTINPUT8 WindowsTelecontrolFactory::sDirectInput;
QStringList WindowsTelecontrolFactory::disallowedControllerNames = QStringList() << "3Dconnexion KMJ Emulator";

WindowsTelecontrolFactory::WindowsTelecontrolFactory()
{
	sDirectInput = 0;
    sGamepadDevices = QMap<QString, Gamepad *>();
    DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&sDirectInput, NULL );
}

WindowsTelecontrolFactory::~WindowsTelecontrolFactory()
{
    try{
        if(sDirectInput){
            sDirectInput->Release();
        }
    }
    catch(...){}
}

WindowsTelecontrolFactory &WindowsTelecontrolFactory::instance()
{
    static WindowsTelecontrolFactory factory;
    return factory;
}

QMap<QString, Gamepad *> WindowsTelecontrolFactory::getGamepadDevices()
{
    // force constructor/destructor call to acquire/release sDirectInput
    instance();
    sGamepadDevices = QMap<QString, Gamepad *>();
    sDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, WindowsTelecontrolFactory::enumDevices, 0, DIEDFL_ATTACHEDONLY);
    return sGamepadDevices;
}

BOOL CALLBACK WindowsTelecontrolFactory::enumDevices(const DIDEVICEINSTANCE *inst, void *)
{
    try
    {
        QString name = QString::fromWCharArray(inst->tszInstanceName).trimmed();
        QString guid = QUuid(inst->guidProduct).toString().replace('{',"").replace('}',"");

        if(disallowedControllerNames.contains(name)){
            qDebug() << tr("Device not supported:") << " " << name;
            return DIENUM_CONTINUE;
        }

        WindowsGamepad* gamepad = new WindowsGamepad(name, guid);
        HRESULT hr = sDirectInput->CreateDevice(inst->guidInstance, &gamepad->mDevice, NULL);
        if(FAILED(hr) || !gamepad->initialize()){
            gamepad->deleteLater();
            throw std::runtime_error(qPrintable(tr("Error initializing gamepad: %1").arg(name)));
        } else {
            for(int i=1; i<=99; i++){
                if(i >= 2){
                    name = name + " " + QString::number(i);
                }
                if(sGamepadDevices.contains(name)){
                    continue;
                }
                sGamepadDevices[name] = gamepad;
                break;
            }
        }
    }
    catch(const std::exception& e)
    {
        qWarning() << tr("Could not create device:") << " " << e.what();
    }
    catch(...)
    {
        qWarning() << tr("Could not create device");
    }
    return DIENUM_CONTINUE;
}
