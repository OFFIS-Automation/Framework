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
    explicit NotificationMainWidget(QWidget *parent, bool useAnimations);
    ~NotificationMainWidget();

    void setAnimationsEnabled(bool enabled);

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
    bool mAnimate;
};

#endif // NOTIFICATIONMAINWIDGET_H
