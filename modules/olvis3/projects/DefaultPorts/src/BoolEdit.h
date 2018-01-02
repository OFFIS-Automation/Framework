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

#ifndef BOOLEDIT_H
#define BOOLEDIT_H

#include "AbstractPortEditWidget.h"
#include <QPushButton>

class BoolEdit : public AbstractPortEditWidget
{
    Q_OBJECT

public:
    explicit BoolEdit(QWidget *parent = 0);
    virtual QString asString();
    virtual void onStartEdit();
    virtual ~BoolEdit();
    virtual QVariant editValue(bool&) { return mValue; }

private slots:
    void on_yes_clicked();
    void on_no_clicked();
    void onButtonClicked(bool yesButton);

protected:
    QPushButton* mYesButton, *mNoButton;
};

#endif // BOOLEDIT_H
