#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H


#include <QString>
#include <QPixmap>
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
    static void newInfo(const QString& text, uint durationMs = 3000, const QPixmap &pixmap = QPixmap());
    static void newWarning(const QString& text, uint durationMs = 3000, const QPixmap &pixmap = QPixmap());
    static void newError(const QString& text, uint durationMs = 3000, const QPixmap &pixmap = QPixmap());

private:
    Notifications();
    static Notifications& instance();
    NotificationMainWidget* mWidget;
    QMutex mMutex;
};

inline void notifyInfo(const QString& text, uint durationMs = 3000, const QPixmap& pixmap = QPixmap())
{
    Notifications::newInfo(text, durationMs, pixmap);
}

inline void notifyInfo(const QString& text, const QPixmap& pixmap)
{
    Notifications::newInfo(text, 3000, pixmap);
}

inline void notifyWarning(const QString& text, uint durationMs = 3000, const QPixmap& pixmap = QPixmap())
{
    Notifications::newWarning(text, durationMs, pixmap);
}

inline void notifyWarning(const QString& text, const QPixmap& pixmap)
{
    Notifications::newWarning(text, 3000, pixmap);
}

inline void notifyError(const QString& text, uint durationMs = 3000, const QPixmap& pixmap = QPixmap())
{
    Notifications::newError(text, durationMs, pixmap);
}

inline void notifyError(const QString& text, const QPixmap& pixmap)
{
    Notifications::newError(text, 3000, pixmap);
}


#endif // NOTIFICATIONS_H
