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

