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

#ifndef ABSTRACTPORTEDITWIDGET_H
#define ABSTRACTPORTEDITWIDGET_H

#include "AbstractPortEdit.h"
#include <QWidget>

#include "ui_AbstractPortEditWidget.h"

class AbstractPortEditWidget : public QWidget, public virtual AbstractPortEdit
{
    Q_OBJECT
public:
    AbstractPortEditWidget(QWidget* parent = 0);
    virtual ~AbstractPortEditWidget();
    virtual QWidget* editWidget() { return this; }
    virtual QVariant editValue(bool&){ return QVariant(); }

private slots:
    void on_cancel_clicked();
    void on_submit_clicked();

protected:
    void submit();
    Ui::AbstractPortEditWidget* ui;
    void keyPressEvent(QKeyEvent *ev);
};


#endif // ABSTRACTPORTEDITWIDGET_H
