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

    void newMessage(const QString text, uint durationMSec, const QPixmap &pixmap, int type, bool animate);
signals:
    void finished(NotificationWidget*);
protected slots:
    void onAnimationFinished();
protected:
    void startAnimation(uint pauseMsec = 5000, bool useAnimation = true);
    void setVisible(bool visible);
private:
    Ui::NotificationWidget *ui;
};

#endif // NOTIFICATIONWIDGET_H
