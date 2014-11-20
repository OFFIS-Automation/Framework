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

#include "ScriptInput.h"
#include "ui_ScriptInput.h"

#include <core/UserRequest.h>

#include <QDebug>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>

#include "ScriptStringInput.h"

class ScriptButton : public QPushButton
{
public:
    ScriptButton(const QString& title, int id) : QPushButton(title), scriptId(id) {}
    const int scriptId;
};

class ScriptChoiceInput : public QComboBox, public ScriptInputItem
{
public:
    ScriptChoiceInput(const QStringList& names, const QVariantList& values, const QVariant& value) {
        for(int i=0;i<names.size(); i++)
        {
            addItem(names[i], values.value(i));
            if(values.value(i) == value)
                setCurrentIndex(i);
        }
    }
    QVariant getValue() { return itemData(currentIndex()); }
};


class ScriptStringChoiceInput : public QComboBox, public ScriptInputItem
{
public:
    ScriptStringChoiceInput(const QString& data, QStringList choices) {
        addItems(choices);
        setEditable(true);
        setEditText(data);
    }
    QVariant getValue() { return currentText(); }
};

class ScriptIntInput : public QSpinBox, public ScriptInputItem
{
public:
    QVariant getValue() { return value(); }
};

class ScriptDoubleInput : public QLineEdit, public ScriptInputItem
{
public:
    ScriptDoubleInput(double value) : QLineEdit(QString("%1").arg(value)) {setValidator(new QDoubleValidator(this));}
    QVariant getValue() { return text().toDouble(); }
};

class ScriptBoolInput : public QWidget, public ScriptInputItem
{
public:
    ScriptBoolInput(bool value, QString trueName, QString falseName) : mTrue(new QRadioButton(trueName)), mFalse(new QRadioButton(falseName))
    {
        QHBoxLayout* l = new QHBoxLayout();
        setLayout(l);
        l->addWidget(mTrue);
        l->addWidget(mFalse);
        l->addStretch(1);
        if(value)
            mTrue->setChecked(true);
        else
            mFalse->setChecked(true);
    }
    QVariant getValue() { return mTrue->isChecked() ? true : false; }
    QRadioButton* mTrue, *mFalse;
};

ScriptInput::ScriptInput(const UserRequest &input, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ScriptInput)
{
    ui->setupUi(this);
    ui->title->setText(input.title);
    mUid = input.uid;
    foreach(const UserRequestItem& item, input.items)
    {
        if(item.constraints.contains("choices"))
        {
            QVariantList values = item.constraints.value("choices").toList();
            QStringList names= item.constraints.value("names").toStringList();
            ScriptChoiceInput* input = new ScriptChoiceInput(names, values, item.value);
            ui->inputs->addRow(item.desc, input);
            mInputs.append(input);
        }
        if(item.value.type() == QVariant::String && item.constraints.contains("stringChoices"))
        {
            ScriptStringChoiceInput* input = new ScriptStringChoiceInput(item.value.toString(), item.constraints["stringChoices"].toStringList());
            ui->inputs->addRow(item.desc, input);
            mInputs.append(input);
        }
        else if(item.value.type() == QVariant::String)
        {
            ScriptStringInput* input = new ScriptStringInput(item.value.toString(), item.constraints);
            ui->inputs->addRow(item.desc, input);
            mInputs.append(input);
        }
        else if(item.value.type() == QVariant::Int)
        {
            ScriptIntInput* input = new ScriptIntInput();
            // Overwrite range
            input->setRange(item.constraints["min"].toInt(), item.constraints["max"].toInt());
            input->setSingleStep(item.constraints["stepping"].toInt());
            // Set value, hopefully in range
            input->setValue(item.value.toInt());
            ui->inputs->addRow(item.desc, input);
            mInputs.append(input);
        }
        else if(item.value.type() == QVariant::Double)
        {
            ScriptDoubleInput* input = new ScriptDoubleInput(item.value.toDouble());
            ui->inputs->addRow(item.desc, input);
            mInputs.append(input);
        }

        else if(item.value.type() == QVariant::Bool)
        {
            QString trueName = item.constraints["trueName"].toString();
            QString falseName = item.constraints["falseName"].toString();
            ScriptBoolInput* input = new ScriptBoolInput(item.value.toBool(), trueName, falseName);
            ui->inputs->addRow(item.desc, input);
            mInputs.append(input);
        }
    }
    foreach(const UserRequestButton& item, input.buttons)
    {
        ScriptButton* button = new ScriptButton(item.name, item.id);
        ui->buttons->addWidget(button);
        connect(button, SIGNAL(clicked()), SLOT(buttonTriggered()));
    }
}


ScriptInput::~ScriptInput()
{
    delete ui;
}

void ScriptInput::buttonTriggered()
{
    QPushButton* pushbutton = qobject_cast<QPushButton*>(sender());
    ScriptButton* button = static_cast<ScriptButton*>(pushbutton);
    if(!button)
        return;
    QList<QVariant> data;
    foreach(ScriptInputItem* item, mInputs)
    {
        data << item->getValue();
    }
    emit newUserInput(mUid, button->scriptId, data);
}
