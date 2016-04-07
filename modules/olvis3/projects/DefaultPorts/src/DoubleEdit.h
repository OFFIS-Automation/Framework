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

#ifndef DOUBLEEDIT_H
#define DOUBLEEDIT_H

#include "AbstractPortEditWidget.h"

#include <QDoubleSpinBox>

class DoubleEdit : public AbstractPortEditWidget
{
    Q_OBJECT
public:
    explicit DoubleEdit(QWidget *parent = 0);
    virtual ~DoubleEdit();
    virtual void onStartEdit();
    virtual QString asString();
    virtual QVariant editValue(bool& ok);

private slots:
    void on_spinBox_editingFinished();

private:
    QDoubleSpinBox* mSpinBox;
};

#endif // DOUBLEEDIT_H
