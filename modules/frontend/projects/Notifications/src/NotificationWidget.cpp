// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

void NotificationWidget::newMessage(const QString text, uint durationMs, const QPixmap& pixmap, int type, bool animate)
{
    switch(type)
    {
    case ErrorType:
        ui->errorWidget->show();
        ui->errorText->setText(text);
        if(!pixmap.isNull())
            ui->errorIcon->setPixmap(pixmap);

        break;
    case WarningType:
        ui->warningWidget->show();
        ui->warningText->setText(text);
        if(!pixmap.isNull())
            ui->warningIcon->setPixmap(pixmap);

        break;
    default:
        ui->infoWidget->show();
        ui->infoText->setText(text);
        if(!pixmap.isNull())
            ui->infoIcon->setPixmap(pixmap);
        break;
    }
    show();
    startAnimation(durationMs, animate);
}

void NotificationWidget::onAnimationFinished()
{
    emit finished(this);
}



void NotificationWidget::startAnimation(uint pauseMsec, bool useAnimation)
{
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup();
    if(useAnimation)
    {
        QPropertyAnimation* anim = new QPropertyAnimation(this, "maximumHeight");
        anim->setStartValue(0);
        anim->setEndValue(42);
        anim->setDuration(500);
        group->addAnimation(anim);
    }
    group->addPause(pauseMsec);
    if(useAnimation)
    {
        QPropertyAnimation* anim2 = new QPropertyAnimation(this, "maximumHeight");
        anim2->setStartValue(42);
        anim2->setEndValue(0);
        anim2->setDuration(500);
        group->addAnimation(anim2);
    }
    connect(group, SIGNAL(finished()), SLOT(onAnimationFinished()));
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void NotificationWidget::setVisible(bool visible)
{
    QWidget::setVisible(visible);
}


