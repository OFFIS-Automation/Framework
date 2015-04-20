#include "NotificationMainWidget.h"
#include "NotificationWidget.h"
#include "ui_NotificationMainWidget.h"

NotificationMainWidget::NotificationMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotificationMainWidget)
{
    connect(this, SIGNAL(newMessage(QString,uint,int)), SLOT(onNewMessage(QString,uint,int)));
    ui->setupUi(this);
    resize(5000, 0);
    show();
}


NotificationMainWidget::~NotificationMainWidget()
{

}


void NotificationMainWidget::onNewMessage(const QString &text, uint duration, int type)
{
    show();
    NotificationWidget* subWidget = new NotificationWidget(this);
    ui->notificationLayout->insertWidget(0, subWidget);
    connect(subWidget, SIGNAL(finished(NotificationWidget*)), SLOT(onChildFinished(NotificationWidget*)));
    subWidget->newMessage(text, duration, type);
    resize(5000, height() + 42);
    update();
}

void NotificationMainWidget::onChildFinished(NotificationWidget *widget)
{
    widget->hide();
    ui->notificationLayout->removeWidget(widget);
    widget->deleteLater();
    resize(5000, height() - 42);
    parentWidget()->update();
}

