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

#include "ScriptInfoPanel.h"
#include "ui_ScriptInfoPanel.h"



ScriptInfoPanel::ScriptInfoPanel(const QString &title, const QStringList &names, QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ScriptInfoPanel)
{
    ui->setupUi(this);
    setTitle(title);
    foreach(const QString& name, names)
    {
        QLabel* label= new QLabel();
        label->setFrameStyle(QFrame::Sunken);
        label->setFrameShape(QFrame::Box);
        mValues << label;
        ui->formLayout->addRow(name, label);
    }
}

ScriptInfoPanel::~ScriptInfoPanel()
{
    delete ui;
}

void ScriptInfoPanel::updateValues(const QStringList &values)
{
    for(int i=0; i<values.size(); i++)
    {
        QLabel* edit = mValues.value(i, 0);
        if(!edit)
            return;
        edit->setText(values[i]);
    }
}
