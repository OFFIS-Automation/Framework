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

#ifndef NUMBEREDIT_H
#define NUMBEREDIT_H

#include "AbstractPortEditWidget.h"

#include <QSpinBox>

class NumberEdit : public AbstractPortEditWidget
{
    Q_OBJECT

public:
    explicit NumberEdit(QWidget *parent = 0);
    virtual ~NumberEdit();
    virtual void onStartEdit();
    virtual QString asString();
    virtual QVariant editValue(bool& ok);

private:
    QSpinBox* mSpinBox;
};

#endif // NUMBEREDIT_H
