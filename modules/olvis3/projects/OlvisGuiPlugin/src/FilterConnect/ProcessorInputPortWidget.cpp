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

#include "ProcessorInputPortWidget.h"

#include <QInputDialog>
#include "../OlvisSingleton.h"

ProcessorInputPortWidget::ProcessorInputPortWidget(const FilterGroupPort &info, bool renameAllowed, QWidget *parent)
    : FilterPortWidget(info.parentId, info.port, parent)
{
    setNewName(info.name);
    mPortInfo = info;
    setStyleSheet("background-color: #f0a0a0;");
    mRenameAllowed = renameAllowed;
    connect(this, SIGNAL(rename(PortId, QString)), &OlvisSingleton::instance(), SLOT(renameInput(PortId, QString)));
}

void ProcessorInputPortWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    if(!mRenameAllowed)
        return;
    QString newName = QInputDialog::getText(0, tr("Change port name"), tr("Enter new name"), QLineEdit::Normal, mPortInfo.name);
    if(!newName.isEmpty())
        emit rename(PortId(mPortInfo.filterId, mInfo.name), newName);

}
