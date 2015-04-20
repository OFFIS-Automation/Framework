#include "NotificationWidget.h"
#include "ui_NotificationWidget.h"

#include <QPauseAnimation>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>


NotificationWidget::NotificationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotificationWidget)
{
    ui->setupUi(this);
    ui->errorWidget->hide();
    ui->warningWidget->hide();
    ui->infoWidget->hide();
    resize(0,0);
}


NotificationWidget::~NotificationWidget()
{
    delete ui;
}

void NotificationWidget::newMessage(const QString text, uint durationMs, int type)
{
    switch(type)
    {
    case ErrorType:
        ui->errorWidget->show();
        ui->errorText->setText(text);
        break;
    case WarningType:
        ui->warningWidget->show();
        ui->warningText->setText(text);
        break;
    default:
        ui->infoWidget->show();
        ui->infoText->setText(text);
        break;
    }
    show();
    startAnimation(durationMs);
}

void NotificationWidget::onAnimationFinished()
{
    emit finished(this);
}



void NotificationWidget::startAnimation(uint pauseMsec)
{
    QPropertyAnimation* anim = new QPropertyAnimation(this, "maximumHeight");
    anim->setStartValue(0);
    anim->setEndValue(42);
    anim->setDuration(500);
    QPropertyAnimation* anim2 = new QPropertyAnimation(this, "maximumHeight");
    anim2->setStartValue(42);
    anim2->setEndValue(0);
    anim2->setDuration(500);
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup();
    group->addAnimation(anim);
    group->addPause(pauseMsec);
    group->addAnimation(anim2);
    connect(group, SIGNAL(finished()), SLOT(onAnimationFinished()));
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void NotificationWidget::setVisible(bool visible)
{
    QWidget::setVisible(visible);
}


