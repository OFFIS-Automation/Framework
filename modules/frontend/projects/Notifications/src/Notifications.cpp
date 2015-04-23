#include "../include/Notifications.h"
#include "NotificationMainWidget.h"

#include <QWidget>


Notifications::Notifications()
{
    mWidget = 0;
}

void Notifications::setMainWindow(QWidget *mainWindow, bool useAnimations)
{
    QMutexLocker locke(&instance().mMutex);
    if(instance().mWidget != 0)
    {
        instance().mWidget->setParent(0);
        delete instance().mWidget;
    }
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

