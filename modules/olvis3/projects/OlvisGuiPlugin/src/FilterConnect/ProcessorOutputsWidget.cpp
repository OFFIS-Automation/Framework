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

#include "ProcessorOutputsWidget.h"
#include "ui_FilterWidget.h"

#include "ProcessorOutputPortWidget.h"
#include <src/OlvisSingleton.h>

ProcessorOutputsWidget::ProcessorOutputsWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FilterWidget)
{
    ui->setupUi(this);
    ui->name->setText("outputs");
    mTemp = 0;
}

ProcessorOutputsWidget::~ProcessorOutputsWidget()
{
    delete ui;
}

void ProcessorOutputsWidget::addTempPort(bool left, int filterId, const QString &portId)
{
    if(!mTemp)
    {
        PortInfo pinfo = OlvisSingleton::instance().getPortInfo(filterId, portId);
        mTemp = new ProcessorOutputPortWidget(filterId, pinfo, this);
        mTemp->setNewName(portId);
        if(left)
            ui->leftInputs->addWidget(mTemp);
        else
            ui->rightInputs->addWidget(mTemp);
    }
}

void ProcessorOutputsWidget::clearTempPort()
{
    if(mTemp)
    {
        ui->leftInputs->removeWidget(mTemp);
        ui->rightInputs->removeWidget(mTemp);
        delete mTemp;
        mTemp = 0;
    }
}

ProcessorOutputPortWidget* ProcessorOutputsWidget::addOutput(int filterId, const QString &portId, const QString &name)
{
    PortInfo pinfo = OlvisSingleton::instance().getPortInfo(filterId, portId);
    ProcessorOutputPortWidget* w = new ProcessorOutputPortWidget(filterId, pinfo, this);
    connect(w, SIGNAL(startConnect(FilterPortWidget*)), SIGNAL(startConnect(FilterPortWidget*)));
    connect(w, SIGNAL(startDisconnect(FilterPortWidget*)), SIGNAL(startDisconnect(FilterPortWidget*)));
    w->setNewName(name);
    if(pinfo.isMainType)
        ui->leftInputs->addWidget(w);
    else
        ui->rightInputs->addWidget(w);
    return w;
}

ProcessorOutputPortWidget* ProcessorOutputsWidget::takeOutput(int filterId, const QString &portId)
{
    foreach(ProcessorOutputPortWidget* port, findChildren<ProcessorOutputPortWidget*>())
    {
        if(port->filterId() == filterId && port->portName() == portId)
        {
            ui->leftInputs->removeWidget(port);
            ui->rightInputs->removeWidget(port);
            return port;
        }
    }
    return 0;
}

void ProcessorOutputsWidget::renameOutput(int filterId, const QString &portId, const QString &name)
{
    foreach(ProcessorOutputPortWidget* port, findChildren<ProcessorOutputPortWidget*>())
    {
        if(port->filterId() == filterId && port->portName() == portId)
        {
            port->setNewName(name);
            break;
        }
    }
}
