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

#include "AbstractPortEditWidget.h"

#include <QKeyEvent>

AbstractPortEditWidget::AbstractPortEditWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::AbstractPortEditWidget)
{
    ui->setupUi(this);
}

AbstractPortEditWidget::~AbstractPortEditWidget()
{
    delete ui;
}

void AbstractPortEditWidget::setAutoSubmit(bool autoSubmit)
{
    mAutoSubmit = autoSubmit;
    ui->submit->setHidden(mAutoSubmit);
    ui->cancel->setHidden(mAutoSubmit);
}

void AbstractPortEditWidget::on_cancel_clicked()
{
    editCanceled();
}

void AbstractPortEditWidget::on_submit_clicked()
{
    submit();
}

void AbstractPortEditWidget::submit()
{
    bool ok = true;
    QVariant data = editValue(ok);
    if(ok)
        editFinished(data);
}

void AbstractPortEditWidget::keyPressEvent(QKeyEvent *ev)
{
    if((ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return) && ev->modifiers() == Qt::NoModifier)
    {
        submit();
    }
    if(ev->key() == Qt::Key_Escape && ev->modifiers() == Qt::NoModifier)
    {
        editCanceled();
    }
}
