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

#include "ChoiceEdit.h"

ChoiceEdit::ChoiceEdit(QWidget *parent) :
    AbstractPortEditWidget(parent)
{
    mComboBox = new QComboBox(this);
    ui->layout->insertWidget(0, mComboBox);

    // Connect signal
    connect(mComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
}

ChoiceEdit::~ChoiceEdit()
{
}

void ChoiceEdit::setInfo(const PortInfo &info)
{
    QList<QVariant> values = info.constraints.value("choices").toList();
    QList<QVariant> currentValues = mInfo.constraints.value("choices").toList();

    QStringList names = info.constraints.value("choiceNames").toStringList();
    QStringList currentNames = mInfo.constraints.value("choiceNames").toStringList();

    if(values != currentValues || names != currentNames){
        bool oldState = mComboBox->blockSignals(true);

        mComboBox->clear();
        mNames.clear();
        mValues.clear();

        AbstractPortEditWidget::setInfo(info);

        QList<QVariant> values = mInfo.constraints.value("choices").toList();
        QStringList names = mInfo.constraints.value("choiceNames").toStringList();
        for(int i=0;i<values.size(); i++){
            const QVariant& value = values[i];
            QString name;
            if(names.size() > i){
                name = names[i];
            } else {
                name = value.toString();
            }
            mValues.append(value);
            mNames.append(name);
            mComboBox->addItem(name, value);
        }

        mComboBox->blockSignals(oldState);
    }
}

QString ChoiceEdit::asString()
{
    int index = mValues.indexOf(mValue);
    if(index >= 0){
        return mNames[index];
    }
    return "";
}

void ChoiceEdit::onStartEdit()
{
    // Select the correct item
    bool oldState = mComboBox->blockSignals(true);
    mComboBox->setCurrentIndex(mComboBox->findData(mValue));
    mComboBox->blockSignals(oldState);
}

QVariant ChoiceEdit::editValue(bool&)
{
    return mComboBox->itemData(mComboBox->currentIndex());
}

void ChoiceEdit::onCurrentIndexChanged(int index)
{
    if(mAutoSubmit){
        editFinished(mComboBox->itemData(index));
    }
}
