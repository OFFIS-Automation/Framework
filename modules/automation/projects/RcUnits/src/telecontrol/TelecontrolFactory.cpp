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

#include "TelecontrolFactory.h"
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QPluginLoader>

#ifdef Q_OS_WIN
#include "WindowsGamepadFactory.h"
#elif defined(Q_OS_LINUX)
#include "LinuxGamepadFactory.h"
#endif

TelecontrolFactory::TelecontrolFactory(QObject *parent) :
    QObject(parent)
{
    // Check telecontrol folder for haptic plugins
    QDir dir(QCoreApplication::applicationDirPath() + "/plugins/telecontrol");
    QStringList suffixFilter = QStringList("*.dll") << "*.so" << "*.dylib"; // Filter for DLLS (windows) and so/dylib (UNIX)
    QStringList files = dir.entryList(suffixFilter, QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
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
                mHapticInterfaces[hapticInterface->name()] = hapticInterface;
            }
        }
    }
}

TelecontrolFactory &TelecontrolFactory::instance()
{
    static TelecontrolFactory factory;
    return factory;
}

Gamepad *TelecontrolFactory::createGamepad()
{
    Gamepad* gamepad = 0;

    #ifdef Q_OS_WIN
        gamepad = WindowsGamepadFactory::createGamepad();
    #elif defined(Q_OS_LINUX)
        gamepad = LinuxGamepadFactory::createGamepad();
    #endif

    return gamepad;
}

QMap<QString, HapticInterface *> TelecontrolFactory::getHapticInterfaces()
{
    return instance().mHapticInterfaces;
}
