// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#include "FilterPortWidget.h"
#include "ui_FilterPortWidget.h"
#include <core/PortInfo.h>
#include <core/PortId.h>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>


FilterPortWidget::FilterPortWidget(int filterId, const PortInfo& info, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FilterPortWidget()),
    mFilterId(filterId),
    mInfo(info)
{
    ui->setupUi(this);
    ui->text->setToolTip(info.desc);
    setNewName(info.name);
    ui->icon->setPixmap(QPixmap::fromImage(info.icon).scaled(16,16, Qt::KeepAspectRatio));
    ui->icon->setToolTip(info.typeName);
    setFrameShadow(QFrame::Sunken);
    setFrameShape(QFrame::Box);
    setFixedSize(120,18);
}

PortId FilterPortWidget::portId() const
{
    return PortId(filterId(), portName());
}

void FilterPortWidget::startDrag()
{
    if(!draggable())
        return;
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    QStringList data;
    if(isOutput())
        data << "output";
    else
        data << "input";
    data << QByteArray::number(filterId());
    data << portName().toLocal8Bit();
    data << QByteArray::number(portType());

    mimeData->setData("application/x-olvis-port", data.join("/").toLocal8Bit());
    drag->setPixmap(QPixmap::grabWidget(this));
    drag->setHotSpot(QPoint(width()/2, height()/2));
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);
}


void FilterPortWidget::mousePressEvent(QMouseEvent *ev)
{
    mPressedButton = ev->button();
    mStartPos = ev->pos();
}

void FilterPortWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if(!mStartPos.isNull() && (ev->pos() - mStartPos).manhattanLength() > QApplication::startDragDistance())
    {
        if(mPressedButton == Qt::LeftButton)
            startDrag();
        else if(mPressedButton == Qt::RightButton)
        {
            if(isOutput())
                emit startConnect(this);
            else
                emit startDisconnect(this);
        }
        mStartPos = QPoint();
    }
    else
        ev->ignore();
}

void FilterPortWidget::setNewName(QString name)
{
    if(mInfo.isArray)
        name += "[]";
    ui->text->setText(name);
}
