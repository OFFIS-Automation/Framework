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

#ifndef PROCESSORINPUTPORTWIDGET_H
#define PROCESSORINPUTPORTWIDGET_H

#include "FilterPortWidget.h"

#include <core/ProcessorInfo.h>

class ProcessorInputPortWidget : public FilterPortWidget
{
    Q_OBJECT
public:
    explicit ProcessorInputPortWidget(const FilterGroupPort& info, bool renameAllowed = false, QWidget *parent = 0);
    bool isOutput() { return true; }
    virtual bool draggable() { return false; }
    FilterGroupPort info2() const { return mPortInfo; }
    QString realName() { return info2().name; }
signals:
    void rename(const PortId& port, const QString& newName);
protected:
    void mouseDoubleClickEvent(QMouseEvent *);
    FilterGroupPort mPortInfo;
    bool mRenameAllowed;
};

#endif // PROCESSORINPUTPORTWIDGET_H
