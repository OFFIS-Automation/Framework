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

#ifndef CHOICEEDIT_H
#define CHOICEEDIT_H

#include "AbstractPortEditWidget.h"

#include <QVariant>
#include <QComboBox>


class ChoiceEdit : public AbstractPortEditWidget
{
    Q_OBJECT

public:
    explicit ChoiceEdit(QWidget *parent = 0);
    virtual ~ChoiceEdit();
    virtual void setInfo(const PortInfo &info);
    virtual QString asString();
    virtual void onStartEdit();
    virtual QVariant editValue(bool& ok);
private:
    QList<QVariant> mValues;
    QList<QString> mNames;
    QComboBox* combo;
};

#endif // CHOICEEDIT_H
