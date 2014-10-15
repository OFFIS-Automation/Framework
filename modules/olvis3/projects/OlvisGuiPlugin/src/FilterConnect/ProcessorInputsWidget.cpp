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

#include "ProcessorInputsWidget.h"
#include "ui_FilterWidget.h"
#include "ProcessorInputPortWidget.h"

ProcessorInputsWidget::ProcessorInputsWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FilterWidget)
{
    ui->setupUi(this);
    ui->name->setText("inputs");
    FilterGroupPort info;
    info.filterId = 0;
    info.name = tr("Connect input ...");
    info.parentId = 0;
    info.port.isMainType = true;
    tmpPortLeft = new ProcessorInputPortWidget(info, false, this);
    info.port.isMainType = false;
    tmpPortRight = new ProcessorInputPortWidget(info, false, this);
    ui->leftOutputs->addWidget(tmpPortLeft);
    ui->rightOutputs->addWidget(tmpPortRight);
    tmpPortLeft->hide();
    tmpPortRight->hide();
    connect(tmpPortLeft, SIGNAL(startConnect(FilterPortWidget*)), SIGNAL(startConnect(FilterPortWidget*)));
    connect(tmpPortLeft, SIGNAL(startDisconnect(FilterPortWidget*)), SIGNAL(startDisconnect(FilterPortWidget*)));
    connect(tmpPortRight, SIGNAL(startConnect(FilterPortWidget*)), SIGNAL(startConnect(FilterPortWidget*)));
    connect(tmpPortRight, SIGNAL(startDisconnect(FilterPortWidget*)), SIGNAL(startDisconnect(FilterPortWidget*)));
}

ProcessorInputsWidget::~ProcessorInputsWidget()
{
    delete ui;
}

void ProcessorInputsWidget::setAllowPortCreation(bool allow)
{
    tmpPortLeft->setVisible(allow);
    tmpPortRight->setVisible(allow);
}

void ProcessorInputsWidget::addInput(const QString &name, int filterId, PortInfo info)
{
    PortId id(filterId, info.name);
    FilterGroupPort port;
    port.port = info;
    port.name = name;
    port.filterId = filterId;
    ProcessorInputPortWidget* portWidget = new ProcessorInputPortWidget(port, true, this);
    if(port.port.isMainType)
        ui->leftOutputs->addWidget(portWidget);
    else
        ui->rightOutputs->addWidget(portWidget);
    mPorts[id] = portWidget;
}

void ProcessorInputsWidget::removeInput(const PortId &target)
{
    ProcessorInputPortWidget* port = mPorts.value(target);
    if(port)
    {
        ui->leftOutputs->removeWidget(port);
        ui->rightOutputs->removeWidget(port);
        mPorts.remove(target);
        delete port;
    }
}

QString ProcessorInputsWidget::nameOf(const PortId &target)
{
    ProcessorInputPortWidget* port = mPorts.value(target);
    if(port)
        return port->realName();
    return QString();
}

QList<ProcessorInputPortWidget*> ProcessorInputsWidget::updatePorts(QList<FilterGroupPort> portList)
{
    QMap<PortId, ProcessorInputPortWidget*> ports = mPorts;
    foreach(FilterGroupPort port, portList)
    {
        PortId id(port.filterId, port.port.name);
        ProcessorInputPortWidget* portWidget;
        if(ports.contains(id))
            portWidget = ports.take(id);
        else
        {
            portWidget = new ProcessorInputPortWidget(port, false, this);
            connect(portWidget, SIGNAL(startConnect(FilterPortWidget*)), SIGNAL(startConnect(FilterPortWidget*)));
            connect(portWidget, SIGNAL(startDisconnect(FilterPortWidget*)), SIGNAL(startDisconnect(FilterPortWidget*)));
            if(port.port.isMainType)
                ui->leftOutputs->addWidget(portWidget);
            else
                ui->rightOutputs->addWidget(portWidget);
            mPorts[id] = portWidget;
        }
        portWidget->setNewName(port.name);
    }
    // remove old widgets and return then
    QMapIterator<PortId, ProcessorInputPortWidget*> iter(ports);
    while(iter.hasNext())
    {
        iter.next();
        mPorts.remove(iter.key());
        ui->leftOutputs->removeWidget(iter.value());
        ui->rightOutputs->removeWidget(iter.value());
    }
    return ports.values();
}
