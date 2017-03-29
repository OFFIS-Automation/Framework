// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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
#include "WindowsGamepad.h"
#include "WindowsConnexionGamepad.h"

#include <telecontrol/GamepadInterface.h>

#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QWidget>
#include <QApplication>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QUuid>
#include <QMainWindow>

// DirectX
#include <dinput.h>
// 3DxWare
#include <spwmacro.h>
#include <si.h>
#include <siapp.h>

QMap<QString, Gamepad *> WindowsTelecontrolFactory::sGamepadDevices;
LPDIRECTINPUT8 WindowsTelecontrolFactory::sDirectInput;
QStringList WindowsTelecontrolFactory::disallowedControllerNames = QStringList() << "3Dconnexion" << "SpaceMouse" << "SpaceNavigator";

WindowsTelecontrolFactory::WindowsTelecontrolFactory()
{
    sDirectInput = 0;
    sGamepadDevices = QMap<QString, Gamepad *>();

    // 1. Search for DirectX supported devices
    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&sDirectInput, NULL );
    sDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, WindowsTelecontrolFactory::enumDevices, 0, DIEDFL_ATTACHEDONLY);

    // 2. Search for 3Dconnexion devices
    enumConnexionDevices();
}

WindowsTelecontrolFactory::~WindowsTelecontrolFactory()
{
    try{
        // Shutdown DirectX
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
    return sGamepadDevices;
}

BOOL CALLBACK WindowsTelecontrolFactory::enumDevices(const DIDEVICEINSTANCE *inst, void *)
{
    try
    {
        QString name = QString::fromWCharArray(inst->tszInstanceName).trimmed();
        QString guid = QUuid(inst->guidProduct).toString().replace('{',"").replace('}',"");

        // Check if controller is not allowed
        foreach (QString disallowedControllerName, disallowedControllerNames) {
            if(name.contains(disallowedControllerName)){
                return DIENUM_CONTINUE;
            }
        }

        if(sGamepadDevices.contains(name)){
            // Name duplicate => Append number
            for(int i=2; i<=99; i++){
                QString newName = name + " #" + QString::number(i);
                if(!sGamepadDevices.contains(newName)){
                    name = newName;
                    break;
                }
            }
        }

        // Create instance
        WindowsGamepad* gamepad = new WindowsGamepad(name, guid);
        HRESULT hr = sDirectInput->CreateDevice(inst->guidInstance, &gamepad->mDevice, NULL);
        if(FAILED(hr) || !gamepad->initialize()){
            gamepad->deleteLater();
            throw std::runtime_error(qPrintable(tr("Error initializing gamepad: %1").arg(name)));
        } else {
            sGamepadDevices[name] = gamepad;
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

void WindowsTelecontrolFactory::enumConnexionDevices()
{
    try
    {
        // Get the window handle of the mainWindow
        // This is required ad SiOpenWinInit needs it somehow...
        WId winId;
        foreach(QWidget *widget, QApplication::topLevelWidgets()) {
            QString objectName = widget->objectName();
            if(objectName == "MasterWindow" || objectName == "MainWindow"){
                winId = widget->winId();
                break;
            }
        }
        if(winId > 0){
            // Initialize the API
            if (SiInitialize() == SPW_DLL_LOAD_ERROR){
                throw std::runtime_error(qPrintable(tr("Could not load SiAppDll DLL files")));
            }
            SiOpenData oData;
            SiOpenWinInit(&oData, (HWND)winId);

            // Get all available devices, their devId and allocate them
            int numDevices = SiGetNumDevices();
            for(int devId=1; devId<=numDevices; devId++){
                WindowsConnexionGamepad* connexionGamepad = new WindowsConnexionGamepad(devId, oData);
                if(!connexionGamepad->initialize()){
                    connexionGamepad->deleteLater();
                    throw std::runtime_error(qPrintable(tr("Error initializing connexion gamepad")));
                } else {
                    for(int i=1; i<=99; i++){
                        QString name = connexionGamepad->getName();
                        if(i >= 2){
                            name = name + " " + QString::number(i);
                        }
                        if(sGamepadDevices.contains(name)){
                            continue;
                        }
                        sGamepadDevices[name] = connexionGamepad;
                        break;
                    }
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        qWarning() << tr("Could not create connexion device:") << " " << e.what();
    }
    catch(...)
    {
        qWarning() << tr("Could not create connexion device");
    }
}
