#include "NotificationMainWidget.h"
#include "NotificationWidget.h"
#include "ui_NotificationMainWidget.h"

NotificationMainWidget::NotificationMainWidget(QWidget *parent, bool useAnimations) :
    QWidget(parent),
    ui(new Ui::NotificationMainWidget)
{
    mAnimate = useAnimations;
    connect(this, SIGNAL(newMessage(QString,uint,QPixmap,int)), SLOT(onNewMessage(QString,uint,QPixmap,int)));
    ui->setupUi(this);
    resize(parentWidget()->width(), 0);
    show();
}


NotificationMainWidget::~NotificationMainWidget()
{

}

void NotificationMainWidget::setAnimationsEnabled(bool enabled)
{
    mAnimate = enabled;
}


void NotificationMainWidget::onNewMessage(const QString &text, uint duration, const QPixmap &pixmap, int type)
{
    show();
    NotificationWidget* subWidget = new NotificationWidget(this);
    ui->notificationLayout->insertWidget(0, subWidget);
    connect(subWidget, SIGNAL(finished(NotificationWidget*)), SLOT(onChildFinished(NotificationWidget*)));
    subWidget->newMessage(text, duration, pixmap, type, mAnimate);
    resize(parentWidget()->width(), height() + 42);
    update();
}

void NotificationMainWidget::onChildFinished(NotificationWidget *widget)
{
    widget->hide();
    ui->notificationLayout->removeWidget(widget);
    widget->deleteLater();
    resize(parentWidget()->width(), height() - 42);
    parentWidget()->update();
}

