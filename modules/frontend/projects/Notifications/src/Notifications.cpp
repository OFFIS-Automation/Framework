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

#include "../include/Notifications.h"
#include "NotificationMainWidget.h"

#include <QWidget>


Notifications::Notifications()
{
    mWidget = 0;
}

void Notifications::setMainWindow(QWidget *mainWindow, bool useAnimations)
{
    QMutexLocker lock(&instance().mMutex);
    if(instance().mWidget != 0)
    {
        instance().mWidget->setParent(0);
        delete instance().mWidget;
        instance().mWidget = 0;
    }
    if(mainWindow)
        instance().mWidget = new NotificationMainWidget(mainWindow, useAnimations);
}

void Notifications::setUseAnimations(bool enabled)
{
    QMutexLocker locke(&instance().mMutex);
    if(instance().mWidget != 0)
       instance().mWidget->setAnimationsEnabled(enabled);
}

void Notifications::newInfo(const QString &text, uint durationMs, const QPixmap& pixmap)
{
    QMutexLocker locke(&instance().mMutex);
    if(instance().mWidget)
        instance().mWidget->info(text, durationMs, pixmap);
}

void Notifications::newWarning(const QString &text, uint durationMs, const QPixmap &pixmap)
{
    QMutexLocker locke(&instance().mMutex);
    if(instance().mWidget)
        instance().mWidget->warning(text, durationMs, pixmap);
}

void Notifications::newError(const QString &text, uint durationMs, const QPixmap &pixmap)
{
    QMutexLocker locke(&instance().mMutex);
    if(instance().mWidget)
        instance().mWidget->error(text, durationMs, pixmap);
}

Notifications &Notifications::instance()
{
    static Notifications system;
    return system;
}

