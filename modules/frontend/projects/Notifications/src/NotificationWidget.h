#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H

#include <QWidget>

namespace Ui {
class NotificationWidget;
}

class NotificationWidget : public QWidget
{
    Q_OBJECT

public:
    enum Type
    {
        InfoType,
        WarningType,
        ErrorType
    };

    explicit NotificationWidget(QWidget *parent = 0);
    ~NotificationWidget();

    void newMessage(const QString text, uint durationMSec, const QPixmap &pixmap, int type);
signals:
    void finished(NotificationWidget*);
protected slots:
    void onAnimationFinished();
protected:
    void startAnimation(uint pauseMsec = 5000);
    void setVisible(bool visible);
private:
    Ui::NotificationWidget *ui;
};

#endif // NOTIFICATIONWIDGET_H
