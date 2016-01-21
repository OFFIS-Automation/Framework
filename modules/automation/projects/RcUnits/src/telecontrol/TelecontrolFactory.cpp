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

#include "TelecontrolFactory.h"
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QPluginLoader>

#ifdef Q_OS_WIN

#include "WindowsTelecontrolFactory.h"
#elif defined(Q_OS_LINUX)
#include "LinuxTelecontrolFactory.h"
#endif

TelecontrolFactory::TelecontrolFactory(QObject *parent) :
    QObject(parent)
{
    loadHapticDevices();
}

TelecontrolFactory &TelecontrolFactory::instance()
{
    static TelecontrolFactory factory;
    return factory;
}

QMap<QString, Gamepad *> TelecontrolFactory::getGamepadDevices()
{
    QMap<QString, Gamepad *> gamepadDevices = QMap<QString, Gamepad *>();

    #ifdef Q_OS_WIN
        gamepadDevices = WindowsTelecontrolFactory::getGamepadDevices();
    #elif defined(Q_OS_LINUX)
        #pragma warning(Fix me)
        //gamepad = LinuxTelecontrolFactory::createGamepad();
    #endif

    return gamepadDevices;
}

QMap<QString, HapticDevice *> TelecontrolFactory::getHapticDevices()
{
    return instance().mHapticDevices;
}

// Helper
void TelecontrolFactory::loadHapticDevices()
{
    QMap<QString, HapticDevice *> hapticDevices = QMap<QString, HapticDevice *>();
    // Check telecontrol folder for haptic plugins (haptic plugins are not proviced by default)
    QDir dir(QCoreApplication::applicationDirPath() + "/plugins/telecontrol");
    QStringList suffixFilter = QStringList("*.dll") << "*.so" << "*.dylib"; // Filter for DLLS (windows) and so/dylib (UNIX)
    QStringList files = dir.entryList(suffixFilter, QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
    QString last = QDir::currentPath();
    QDir::setCurrent(QCoreApplication::applicationDirPath() + "/plugins");

    // Iterate all dlls lying around
    foreach(QString file, files){
        // Try to load plugin
        QString absoluteFilePath = dir.absoluteFilePath(file);
        QPluginLoader* loader = new QPluginLoader(absoluteFilePath);
        loader->setLoadHints(QLibrary::ResolveAllSymbolsHint);
        QObject* object = loader->instance();
        if(object){
            // Check if object conforms to HapticInterface protocol (== cast successful)
            HapticInterface* hapticInterface = qobject_cast<HapticInterface *>(object);
            if(hapticInterface){
                foreach(HapticDevice *hapticDevice, hapticInterface->availableDevices()){
                    hapticDevices[hapticDevice->name()] = hapticDevice;
                }
            }
        }
    }
    QDir::setCurrent(last);
    mHapticDevices = hapticDevices;
}

