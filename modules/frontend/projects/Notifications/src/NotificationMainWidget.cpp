// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

