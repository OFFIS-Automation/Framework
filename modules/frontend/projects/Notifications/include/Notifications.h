#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H


#include <QString>
#include <QtCore/qglobal.h>

#if defined(NOTIFICATIONS_LIBRARY)
#  define NOTIFICATIONSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NOTIFICATIONSSHARED_EXPORT Q_DECL_IMPORT
#endif


class QWidget;
class NotificationMainWidget;

class NOTIFICATIONSSHARED_EXPORT Notifications
{

public:
    static void setMainWindow(QWidget* mainWindow);
    static void newInfo(const QString& text, uint durationMs = 3000);
    static void newWarning(const QString& text, uint durationMs = 3000);
    static void newError(const QString& text, uint durationMs = 3000);

private:
    Notifications();
    static Notifications& instance();
    NotificationMainWidget* mWidget;
    QMutex mMutex;
};

inline void notifyInfo(const QString& text, uint durationMs = 3000)
{
    Notifications::newInfo(text, durationMs);
}

inline void notifyWarning(const QString& text, uint durationMs = 3000)
{
    Notifications::newWarning(text, durationMs);
}

inline void notifyError(const QString& text, uint durationMs = 3000)
{
    Notifications::newError(text, durationMs);
}


#endif // NOTIFICATIONS_H
