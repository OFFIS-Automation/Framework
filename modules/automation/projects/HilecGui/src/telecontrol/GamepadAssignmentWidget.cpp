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
    QStringList colors;
    colors << "blue" << "darkGreen" << "lighblue" << "green" << "darkred";

    // get unit
    TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(unit);

    // Buttons
    if(!help.tcGamepadButtons.empty()){
        foreach(RcUnitHelp::TcButton buttonMethod, help.tcGamepadButtons){
            QString labelName = QString("button%1Label").arg(buttonMethod.buttonId);
            QLabel *label = this->findChild<QLabel *>(labelName);
            if(label != NULL){
                QString text = label->text();
                if(text == "Not assigned"){
                    text.clear();
                } else if(!text.isEmpty()){
                    text += "; ";
                }
                label->setText(text + buttonMethod.name);
                label->setStyleSheet("QLabel { color : black; }");
            }
        }
    }

    // Joystick
    if(!help.tcGamepadMoves.empty()){
        int colorCounter = 0;
        foreach(RcUnitHelp::TcMove joystickMethod, help.tcGamepadMoves){
            QString color = colors.value(colorCounter);
            colorCounter = (colorCounter + 1) % colors.size();
            // Deadmans button
            QString labelName = QString("button%1Label").arg(joystickMethod.deadMansButton);
            QLabel *label = this->findChild<QLabel *>(labelName);
            if(label != NULL){
                QString currentText = label->text();
                if(currentText == "Not assigned"){
                    currentText.clear();
                } else if(!currentText.isEmpty()){
                    currentText += "; ";
                }
                QString text = "<span style='color: %1;'>%2</span>";
                label->setText(currentText + text.arg(color, tr("Dead-man's control: ") + joystickMethod.name));
            }
            for(int i=0;i < joystickMethod.joysticks.size(); i++){
                Tc::Joystick joystick = joystickMethod.joysticks[i];
                QString name = joystickMethod.axeNames.value(i);

                // Append method to joystick
                labelName = this->labelNameForJoystick(joystick);
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
                    currentText.append(text.arg(color, joystickMethod.name, name));
                    label->setText(currentText);
                }
            }
        }
    }
}

GamepadAssignmentWidget::~GamepadAssignmentWidget()
{
    delete ui;
}

QString GamepadAssignmentWidget::labelNameForJoystick(Tc::Joystick joystick)
{
    switch (joystick) {
    case Tc::LeftJoystickX:
        return QString("leftStickXLabel");
    case Tc::LeftJoystickY:
        return QString("leftStickYLabel");
    case Tc::RightJoystickX:
        return QString("rightStickXLabel");
    default:
        return QString("rightStickYLabel");
    }

}
