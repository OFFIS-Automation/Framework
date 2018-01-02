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

#include "FilterInfoPortEditWidget.h"
#include "ui_FilterInfoPortEditWidget.h"
#include <core/PortInfo.h>
#include <QDebug>
#include "../PluginContainer.h"

FilterInfoPortEditWidget::FilterInfoPortEditWidget(const PortInfo& info, const QVariant& value, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterInfoPortEditWidget)
{
    ui->setupUi(this);
    mEdit = PluginContainer::getInstance().portEditFor(info);
    mInEditMode = false;
    mPortName = info.name;
    ui->clear->setVisible(info.mode == OptionalPortMode);
    if(mEdit)
    {
        mEdit->setInfo(info);
        setValue(value);
        QWidget* widget = mEdit->editWidget();
        if(widget)
        {
            widget->hide();
            ui->layout->insertWidget(0,widget);
        }
    }
    setConnected(false);
}


FilterInfoPortEditWidget::~FilterInfoPortEditWidget()
{
    if(mEdit)
    {
        QWidget* widget = mEdit->editWidget();
        if(widget)
            ui->layout->removeWidget(widget);
        delete mEdit;

    }
    delete ui;
}

void FilterInfoPortEditWidget::setConnected(bool connected)
{
    mConnected = connected;
    ui->label->setVisible(!connected && mEdit);
    ui->notFound->setVisible(!connected && !mEdit);
    ui->connected->setVisible(connected);
}

void FilterInfoPortEditWidget::setValue(const QVariant &value)
{
    if(mEdit)
    {
        mEdit->setValue(value);
        if(!value.isValid())
        {
            ui->label->clear();
            ui->label->setToolTip(tr("no value set"));
        }
        else
        {
            QString str = mEdit->asString();
            if(str.isEmpty()) str = " ";
            ui->label->setText(str);
            ui->label->setToolTip(str);
        }
    }
}

void FilterInfoPortEditWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    if(!mEdit)
        return;
    if(!mInEditMode)
    {
        mInEditMode = true;
        QWidget* widget = mEdit->editWidget();
        if(widget)
        {
            ui->label->hide();
            ui->connected->hide();
            widget->show();
        }
        mEdit->startEdit(*this);
    }
}

void FilterInfoPortEditWidget::editFinished(const QVariant &variant, const PortInfo &info)
{
    editCanceled(info);
    emit newValue(mPortName, variant);
}

void FilterInfoPortEditWidget::editCanceled(const PortInfo &info)
{
    Q_UNUSED(info);

    if(!mInEditMode)
        return;
    mInEditMode = false;
    QWidget* widget = mEdit->editWidget();
    if(widget)
        widget->hide();
    setConnected(mConnected);
}

void FilterInfoPortEditWidget::on_clear_clicked()
{
    emit newValue(mPortName, QVariant());
}
