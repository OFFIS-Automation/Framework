// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
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

#ifndef FILTERPORTWIDGET_H
#define FILTERPORTWIDGET_H

#include <QLabel>
#include <core/PortInfo.h>

namespace Ui
{
    class FilterPortWidget;
}

struct PortId;

class FilterPortWidget : public QFrame
{
    Q_OBJECT
public:
    explicit FilterPortWidget(int filterId, const PortInfo& info, QWidget *parent = 0);
    int filterId() const { return mFilterId; }
    QString portName() const { return mInfo.name; }
    PortId portId() const;
    bool isLeft() const { return mInfo.isMainType; }
    virtual bool isOutput() = 0;
    virtual bool draggable() = 0;
    int portType() const { return mInfo.type; }
    const PortInfo& getInfo() const { return mInfo; }
    void setNewName(QString name);
signals:
    void startConnect(FilterPortWidget* start);
    void startDisconnect(FilterPortWidget* target);
public slots:

protected:
    void startDrag();
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);

    Qt::MouseButton mPressedButton;
    QPoint mStartPos;


    Ui::FilterPortWidget* ui;
    int mFilterId;
    PortInfo mInfo;
};

#endif // FILTERPORTWIDGET_H
