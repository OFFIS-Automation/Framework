#include "../include/Notifications.h"
#include "NotificationMainWidget.h"

#include <QWidget>


Notifications::Notifications()
{
    mWidget = 0;
}

void Notifications::setMainWindow(QWidget *mainWindow)
{
    QMutexLocker locke(&instance().mMutex);
    if(instance().mWidget != 0)
    {
        instance().mWidget->setParent(0);
        delete instance().mWidget;
    }
    instance().mWidget = new NotificationMainWidget(mainWindow);
}

void Notifications::newInfo(const QString &text, uint durationMs)
{
    QMutexLocker locke(&instance().mMutex);
    if(instance().mWidget)
        instance().mWidget->info(text, durationMs);
}

void Notifications::newWarning(const QString &text, uint durationMs)
{
    QMutexLocker locke(&instance().mMutex);
    if(instance().mWidget)
        instance().mWidget->warning(text, durationMs);
}

void Notifications::newError(const QString &text, uint durationMs)
{
    QMutexLocker locke(&instance().mMutex);
    if(instance().mWidget)
        instance().mWidget->error(text, durationMs);
}

Notifications &Notifications::instance()
{
    static Notifications system;
    return system;
}

