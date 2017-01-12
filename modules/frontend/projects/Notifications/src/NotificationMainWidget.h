// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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
