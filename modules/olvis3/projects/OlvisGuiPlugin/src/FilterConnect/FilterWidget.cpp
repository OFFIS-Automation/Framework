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

#include "FilterWidget.h"
#include "ui_FilterWidget.h"

#include <core/FilterInfo.h>
#include <core/OlvisInterface.h>

#include <QPushButton>
#include <QDebug>
#include <QMenu>

#include "FilterSortingArea.h"
#include "FilterOutputPortWidget.h"
#include "FilterInputPortWidget.h"
#include "../OlvisSingleton.h"

FilterWidget::FilterWidget(const FilterInfo& info, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FilterWidget)
{
    ui->setupUi(this);
    mFilterId = info.id;
    mFilterType = info.typeInfo.uid;
    mDescription = info.typeInfo.desc;
    onFilterRename(info);
    clearError();

    connect(&OlvisSingleton::instance(), SIGNAL(filterRenamed(FilterInfo,QString)), SLOT(onFilterRename(FilterInfo)));

    // Generating Inputs
    QListIterator<PortInfo> inputs(info.typeInfo.inputs);
    while(inputs.hasNext())
    {
        const PortInfo& input = inputs.next();
        FilterInputPortWidget* inWidget = new FilterInputPortWidget(mFilterId, input);
        connect(inWidget, SIGNAL(newPortValue(int,QString,QVariant)), SIGNAL(newPortValue(int,QString,QVariant)));
        connect(inWidget, SIGNAL(startDisconnect(FilterPortWidget*)), SIGNAL(startDisconnect(FilterPortWidget*)));
        connect(inWidget, SIGNAL(startConnect(FilterPortWidget*)), SIGNAL(startConnect(FilterPortWidget*)));

        if(inWidget->isLeft())
            ui->leftInputs->addWidget(inWidget);
        else
            ui->rightInputs->addWidget(inWidget);
    }

    // Generating Outputs
    QListIterator<PortInfo> outputs(info.typeInfo.outputs);
    while(outputs.hasNext())
    {
        const PortInfo& output = outputs.next();
        FilterPortWidget* outWidget = new FilterOutputPortWidget(mFilterId, output);
        connect(outWidget, SIGNAL(startDisconnect(FilterPortWidget*)), SIGNAL(startDisconnect(FilterPortWidget*)));
        connect(outWidget, SIGNAL(startConnect(FilterPortWidget*)), SIGNAL(startConnect(FilterPortWidget*)));
        if(outWidget->isLeft())
            ui->leftOutputs->addWidget(outWidget);
        else
            ui->rightOutputs->addWidget(outWidget);
    }
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::showError(const QString& errorMsg)
{
    setStyleSheet("background-color: #FF5555;");
    setToolTip(errorMsg);
}

void FilterWidget::clearError()
{
    setStyleSheet("background-color: lightgray;");
    setToolTip(mDescription);
}

void FilterWidget::setPortVisibility(int visibility)
{
    foreach(FilterPortWidget* widget, findChildren<FilterPortWidget*>())
    {
        bool visible = widget->getInfo().visibility <= visibility;
        widget->setVisible(visible);
    }
}

void FilterWidget::mousePressEvent(QMouseEvent * event)
{
    QObject* child = childAt(event->pos());
    if(child && qobject_cast<FilterPortWidget*>(child))
    {
        // these are handled externally for port connection
        event->ignore();
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        mDragStartPos = event->pos();
        emit selected(mFilterId);
    }
    else if(event->button() == Qt::RightButton)
        showMenu(event->globalPos());

}

void FilterWidget::mouseMoveEvent(QMouseEvent * event)
{
    if ((event->buttons() & Qt::LeftButton) && !mDragStartPos.isNull())
    {
        if ((event->pos() - mDragStartPos).manhattanLength() > QApplication::startDragDistance())
        {
            mDragStartPos = QPoint();
            FilterSortingArea* parent = qobject_cast<FilterSortingArea*>(parentWidget());
            if(parent)
            {
                QPoint hotspot = event->pos() - parentWidget()->pos();
                parent->startDrag(this, hotspot);
            }
        }
    }
    else
        event->ignore();
}
#
void FilterWidget::on_actionDeleteFilter_triggered()
{
    emit deleteRequest(mFilterId);
}

void FilterWidget::on_actionRename_triggered()
{
    startRename();
}

void FilterWidget::on_name_returnPressed()
{
    QString newName =  ui->name->text();
    endRename();
    emit renameRequested(mFilterId, newName);
}

void FilterWidget::on_name_editingFinished()
{
    endRename();
}

void FilterWidget::showMenu(const QPoint &pos)
{
    QMenu menu;
    menu.addAction(ui->actionDeleteFilter);
    menu.addAction(ui->actionRename);
    menu.exec(pos);
}

void FilterWidget::startRename()
{
    ui->name->setFrame(true);
    ui->name->setReadOnly(false);
    ui->name->selectAll();
    ui->name->setFocus();
}

void FilterWidget::endRename()
{
    ui->name->setFrame(false);
    ui->name->setReadOnly(true);
    ui->name->setText(mName);
}

void FilterWidget::onFilterRename(const FilterInfo &info)
{
    if(info.id == mFilterId)
    {
        ui->name->setText(info.name);
        mName = info.name;
    }
}

void FilterWidget::on_name_doubleClicked()
{
    startRename();
}
