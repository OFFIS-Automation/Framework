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

#include "GamepadAssignmentWidget.h"
#include "ui_GamepadAssignmentWidget.h"

#include <core/RcUnitHelp.h>
#include "../HilecSingleton.h"

GamepadAssignmentWidget::GamepadAssignmentWidget(const QString &unit, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamepadAssignmentWidget)
{
    ui->setupUi(this);

    // Colors
    QStringList colors = QStringList() << "blue" << "darkGreen" << "darkred"  << "cyan" << "green";

    // get unit
    TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(unit);

    // Buttons
    if(!help.tcButtonMethods.empty()){
        foreach(RcUnitHelp::TcButton buttonMethod, help.tcButtonMethods){
            setButtonLabel(buttonMethod.buttonId, buttonMethod.name);
        }
    }

    // Joystick
    if(!help.tcGamepadMoves.empty()){
        int colorCounter = 0;
        foreach(RcUnitHelp::TcMove joystickMethod, help.tcGamepadMoves){
            QString color = colors.value(colorCounter);
            colorCounter = (colorCounter + 1) % colors.size();
            // Deadmans button
            setButtonLabel(joystickMethod.deadMansButton, "Dead-man's control: " + joystickMethod.name, color);

            for(int i=0;i < joystickMethod.analogDOFs.size(); i++){
                int joystick = joystickMethod.analogDOFs[i];
                QString name = joystickMethod.axeNames.value(i);
                if(joystick == Tc::Gamepad::TriggerJoystick){
                    setButtonLabel(Tc::Gamepad::LeftShoulderLowerButton, QString("%1.%2").arg(joystickMethod.name, name), color);
                    setButtonLabel(Tc::Gamepad::RightShoulderLowerButton, QString("%1.%2").arg(joystickMethod.name, name), color);
                } else {
                    setJoystickLabel(joystick, joystickMethod.name, name, color);
                }
            }
        }
    }
}

GamepadAssignmentWidget::~GamepadAssignmentWidget()
{
    delete ui;
}

QString GamepadAssignmentWidget::labelNameForJoystick(int joystick)
{
    switch (joystick) {
    case Tc::Gamepad::LeftJoystickX:
        return QString("leftStickXLabel");
    case Tc::Gamepad::LeftJoystickY:
        return QString("leftStickYLabel");
    case Tc::Gamepad::RightJoystickX:
        return QString("rightStickXLabel");
    case Tc::Gamepad::RightJoystickY:
        return QString("rightStickYLabel");
    default:
        return QString("Unknown");

    }

}

void GamepadAssignmentWidget::setButtonLabel(int buttonId, const QString &name, const QString& color)
{
    QString labelName = QString("button%1Label").arg(buttonId);
    QLabel *label = this->findChild<QLabel *>(labelName);
    if(label != NULL){
        QString currentText = label->text();
        if(currentText == "Not assigned"){
            currentText.clear();
            label->setStyleSheet("QLabel { color : black; }");
        } else if(!currentText.isEmpty()){
            currentText += "; ";
        }
        QString text = "<span style='color: %1;'>%2</span>";
        label->setText(currentText + text.arg(color, name));
    }
}

void GamepadAssignmentWidget::setJoystickLabel(int joystickId, const QString& unit, const QString name, const QString &color)
{
    // Append method to joystick
    QString labelName = this->labelNameForJoystick(joystickId);
    QLabel *label = this->findChild<QLabel *>(labelName);
    if(label != NULL){
        QString currentText = label->text();
        if(currentText.compare("Not assigned") == 0){
            // Replace placeholder and set correct color
            currentText.clear();
            label->setStyleSheet("QLabel { color : black; }");
        } else {
            if(currentText.length() > 0){
                currentText.append("; ");
            }
        }
        QString text = "<span style='color: %1;'>%2.%3</span>";
        currentText.append(text.arg(color, unit, name));
        label->setText(currentText);
    }
}
