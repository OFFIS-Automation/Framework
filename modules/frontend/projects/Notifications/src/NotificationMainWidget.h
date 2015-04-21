#ifndef NOTIFICATIONMAINWIDGET_H
#define NOTIFICATIONMAINWIDGET_H

#include "NotificationWidget.h"

#include <QWidget>
#include <QTimer>

namespace Ui {
class NotificationMainWidget;
}

class NotificationMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationMainWidget(QWidget *parent = 0);
    ~NotificationMainWidget();
    void info(const QString &text, uint duration, const QPixmap& pixmap)
    { emit newMessage(text, duration, pixmap, NotificationWidget::InfoType); }

    void warning(const QString &text, uint duration, const QPixmap& pixmap)
    { emit newMessage(text, duration, pixmap, NotificationWidget::WarningType); }
    void error(const QString &text, uint duration, const QPixmap& pixmap)
    { emit newMessage(text, duration, pixmap, NotificationWidget::ErrorType); }

signals:
    void newMessage(const QString &text, uint duration, const QPixmap& pixmap, int type);
protected slots:
    void onNewMessage(const QString &text, uint duration, const QPixmap& pixmap, int type);
    void onChildFinished(NotificationWidget* widget);
protected:
    NotificationWidget *createSubWidget();
private:
    Ui::NotificationMainWidget *ui;
};

#endif // NOTIFICATIONMAINWIDGET_H
