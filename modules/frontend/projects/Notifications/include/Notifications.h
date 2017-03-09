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

#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <QMutex>
#include <QString>
#include <QPixmap>
#include <QtCore/qglobal.h>

#if defined(NOTIFICATIONS_LIBRARY)
#  define NOTIFICATIONSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NOTIFICATIONSSHARED_EXPORT Q_DECL_IMPORT
#endif

#define DEFAULT_ANIMATION_DURATION 4000

class QWidget;
class NotificationMainWidget;

class NOTIFICATIONSSHARED_EXPORT Notifications
{

public:
    static void setMainWindow(QWidget* mainWindow, bool useAnimations = true);
    static void setUseAnimations(bool enabled);
    static void newInfo(const QString& text, uint durationMs = 3000, const QPixmap &pixmap = QPixmap());
    static void newWarning(const QString& text, uint durationMs = 3000, const QPixmap &pixmap = QPixmap());
    static void newError(const QString& text, uint durationMs = 3000, const QPixmap &pixmap = QPixmap());

private:
    Notifications();
    static Notifications& instance();
    NotificationMainWidget* mWidget;
    QMutex mMutex;
};

inline void notifyInfo(const QString& text, uint durationMs = DEFAULT_ANIMATION_DURATION, const QPixmap& pixmap = QPixmap())
{
    Notifications::newInfo(text, durationMs, pixmap);
}

inline void notifyInfo(const QString& text, const QPixmap& pixmap)
{
    Notifications::newInfo(text, DEFAULT_ANIMATION_DURATION, pixmap);
}

inline void notifyWarning(const QString& text, uint durationMs = DEFAULT_ANIMATION_DURATION, const QPixmap& pixmap = QPixmap())
{
    Notifications::newWarning(text, durationMs, pixmap);
}

inline void notifyWarning(const QString& text, const QPixmap& pixmap)
{
    Notifications::newWarning(text, DEFAULT_ANIMATION_DURATION, pixmap);
}

inline void notifyError(const QString& text, uint durationMs = DEFAULT_ANIMATION_DURATION, const QPixmap& pixmap = QPixmap())
{
    Notifications::newError(text, durationMs, pixmap);
}

inline void notifyError(const QString& text, const QPixmap& pixmap)
{
    Notifications::newError(text, DEFAULT_ANIMATION_DURATION, pixmap);
}


#endif // NOTIFICATIONS_H
