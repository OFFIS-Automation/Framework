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

#include "ConnexionAssignmentWidget.h"
#include "ui_ConnexionAssignmentWidget.h"

#include <core/RcUnitHelp.h>
#include "../HilecSingleton.h"

ConnexionAssignmentWidget::ConnexionAssignmentWidget(const QString &unit, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnexionAssignmentWidget)
{
    ui->setupUi(this);

    // Colors
    QStringList colors = QStringList() << "blue" << "darkGreen" << "lighblue" << "green" << "darkred";

    // Joystick
    TelecontrolConfig help = HilecSingleton::hilec()->getTelecontrolConfig(unit);
    if(!help.tcGamepadMoves.empty()){
        int colorCounter = 0;
        foreach(RcUnitHelp::TcMove joystickMethod, help.tcGamepadMoves){
            QString color = colors.value(colorCounter);
            colorCounter = (colorCounter + 1) % colors.size();
            for(int i=0;i < joystickMethod.joysticks.size(); i++){
                Tc::Joystick joystick = joystickMethod.joysticks[i];
                QString name = joystickMethod.axeNames.value(i);

                // Append method to joystick
                QString labelName = this->labelNameForJoystick(joystick);
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

ConnexionAssignmentWidget::~ConnexionAssignmentWidget()
{
    delete ui;
}

QString ConnexionAssignmentWidget::labelNameForJoystick(Tc::Joystick joystick)
{
    switch (joystick) {
    case Tc::JoystickX:
        return QString("xLabel");
    case Tc::JoystickY:
        return QString("yLabel");
    case Tc::JoystickZ:
        return QString("zLabel");
    case Tc::JoystickYaw:
        return QString("yawLabel");
    case Tc::JoystickPitch:
        return QString("pitchLabel");
    case Tc::JoystickRoll:
        return QString("rollLabel");
    default:
        return QString();
    }

}
