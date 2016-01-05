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

#include "ChoiceEdit.h"

ChoiceEdit::ChoiceEdit(QWidget *parent) :
    AbstractPortEditWidget(parent)
{
    combo = new QComboBox(this);
    ui->layout->insertWidget(0, combo);
}

ChoiceEdit::~ChoiceEdit()
{
}

void ChoiceEdit::setInfo(const PortInfo &info)
{
    combo->clear();
    mNames.clear();
    mValues.clear();
    AbstractPortEditWidget::setInfo(info);
    QList<QVariant> values = mInfo.constraints.value("choices").toList();
    QStringList names = mInfo.constraints.value("choiceNames").toStringList();
    for(int i=0;i<values.size(); i++)
    {
        const QVariant& value = values[i];
        QString name;
        if(names.size() > i)
            name = names[i];
        else
            name = value.toString();
        mValues.append(value);
        mNames.append(name);
        combo->addItem(name, value);
    }
}

QString ChoiceEdit::asString()
{
    int index = mValues.indexOf(mValue);
    if(index >= 0)
        return mNames[index];
    return "";
}

void ChoiceEdit::onStartEdit()
{
    // select the correct item
    combo->setCurrentIndex(combo->findData(mValue));
    combo->setFocus();
}

QVariant ChoiceEdit::editValue(bool&)
{
    return combo->itemData(combo->currentIndex());
}
