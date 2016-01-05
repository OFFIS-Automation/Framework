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

#ifndef FILTERINPUTPORTWIDGET_H
#define FILTERINPUTPORTWIDGET_H

#include "FilterPortWidget.h"

#include <gui/PortEditInterface.h>

class OlvisInterface;


class FilterInputPortWidget : public FilterPortWidget, public PortEditInterface::Callback
{
    Q_OBJECT
public:
    explicit FilterInputPortWidget(int filterId, const PortInfo& info, QWidget *parent = 0);
    virtual ~FilterInputPortWidget();
    virtual bool isOutput() { return false; }
    virtual bool draggable() { return true; }
signals:
    void newPortValue(int filterId, const QString& portName, const QVariant& value);

protected:
    void editFinished(const QVariant &var);
    void editCanceled();

    virtual void mouseDoubleClickEvent(QMouseEvent*);
    PortEditInterface* mEdit;

};

#endif // FILTERINPUTPORTWIDGET_H
