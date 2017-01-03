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

#include "ProcessorOutputPortWidget.h"
#include <QInputDialog>
#include "../OlvisSingleton.h"
#include "ui_FilterPortWidget.h"

ProcessorOutputPortWidget::ProcessorOutputPortWidget(int filterId, const PortInfo &info, QWidget *parent)
    : FilterPortWidget(filterId, info, parent)
{
    setStyleSheet("background-color: #8080f0;");
    connect(this, SIGNAL(rename(PortId, QString)), &OlvisSingleton::instance(), SLOT(renameProcessorOutput(PortId,QString)));
}


void ProcessorOutputPortWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    QString newName = QInputDialog::getText(0, tr("Change port name"), tr("Enter new name"), QLineEdit::Normal, portName());
    if(!newName.isEmpty())
        emit rename(portId(), newName);
}
