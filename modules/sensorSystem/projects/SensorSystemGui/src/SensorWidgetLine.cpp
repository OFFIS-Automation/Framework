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

#include "SensorWidgetLine.h"
#include "ui_SensorWidgetLine.h"

#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

SensorWidgetLine::SensorWidgetLine(const QString &title) :
    ui(new Ui::SensorWidgetLine)
{
    ui->setupUi(this);
    ui->checkBox->setText(title);
}

SensorWidgetLine::~SensorWidgetLine()
{
    delete ui;
}

void SensorWidgetLine::startDrag()
{
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setData("application/x-sensorSystem-value", name().toLocal8Bit());
    drag->setPixmap(QPixmap::grabWidget(ui->checkBox));
    drag->setHotSpot(QPoint(width()/2, height()/2));
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);
}
void SensorWidgetLine::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
        mStartPos = ev->pos();
    else
        QWidget::mouseMoveEvent(ev);
}

void SensorWidgetLine::mouseReleaseEvent(QMouseEvent *)
{
    mStartPos = QPoint();
}

void SensorWidgetLine::mouseMoveEvent(QMouseEvent *ev)
{
    if(!mStartPos.isNull() && (ev->pos() - mStartPos).manhattanLength() > QApplication::startDragDistance())
        startDrag();
    else
        QWidget::mouseMoveEvent(ev);
}
QString SensorWidgetLine::name()
{
    return ui->checkBox->text();
}

bool SensorWidgetLine::isSelected()
{
    return ui->checkBox->isChecked();
}

void SensorWidgetLine::on_toolButton_clicked()
{
    emit removeTraceItem(name());
}

void SensorWidgetLine::on_checkBox_toggled(bool /* checked */)
{
    emit selectionChanged();
}
