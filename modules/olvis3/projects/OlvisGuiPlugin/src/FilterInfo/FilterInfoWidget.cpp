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

#include "FilterInfoWidget.h"
#include "ui_FilterInfoWidget.h"

#include <core/OlvisInterface.h>
#include <core/FilterInfo.h>

#include <QLabel>
#include <QDebug>
#include <QLineEdit>

#include "FilterInfoPortEditWidget.h"

FilterInfoWidget::FilterInfoWidget(const OlvisInterface& model, QWidget *parent) :
    QDockWidget(parent),
    mInterface(model),
    ui(new Ui::FilterInfoWidget)
{
    ui->setupUi(this);
    // some testing
    connect(&mInterface, SIGNAL(filterCreated(FilterInfo,int)), SLOT(showFilter(FilterInfo)));
    connect(&mInterface, SIGNAL(portValueChanged(int, QString,QVariant)), SLOT(updatePortValue(int, QString, QVariant)));
    connect(&mInterface, SIGNAL(filterConnected(PortId,PortId,int)), SLOT(addPortConnection(PortId, PortId)));
    connect(&mInterface, SIGNAL(filterDisconnected(PortId,PortId,int)), SLOT(removePortConnection(PortId, PortId)));
    connect(&mInterface, SIGNAL(filterRenamed(FilterInfo,QString)), SLOT(onFilterRenamed(FilterInfo)));
    connect(this, SIGNAL(newPortValue(int,QString,QVariant)), &mInterface, SLOT(setPortValue(int,QString,QVariant)), Qt::QueuedConnection);
}

FilterInfoWidget::~FilterInfoWidget()
{
    delete ui;
}

void FilterInfoWidget::showFilter(int id)
{
    FilterInfo info = mInterface.getFilter(id);
    if(info.isValid())
        showFilter(info);
}

void FilterInfoWidget::onFilterRenamed(const FilterInfo &info)
{
    if(info.id == mFilterId)
        showFilter(info);
}

void FilterInfoWidget::showFilter(const FilterInfo& filter)
{
    mFilterId = filter.id;
    QString infoStr = tr("<strong>%1</strong> type: <strong>%2</strong> from <strong>%3</strong><br />%4", "1=name, 2=typeName, 3=pluginName, 4=long description");
    infoStr = infoStr.arg(filter.name, filter.typeInfo.name, filter.typeInfo.plugin, filter.typeInfo.desc);
    ui->nodeInfoText->clear();
    ui->nodeInfoText->appendHtml(infoStr);
    updatePorts(filter);
}

void FilterInfoWidget::clearPorts()
{

    QLayoutItem *child;

    while ((child = ui->portLayout->itemAt(0)) != 0) {
        QWidget* widget = child->widget();
        if(widget)
        {
            ui->portLayout->removeWidget(widget);
            delete widget;
        }
    }
    mPortEdits.clear();

}

void FilterInfoWidget::updatePorts(const FilterInfo &filter)
{
    clearPorts();
    QListIterator<PortInfo> inputs(filter.typeInfo.inputs);
    while(inputs.hasNext())
    {
        const PortInfo& input = inputs.next();
        PortId portId(filter.id, input.name);
        QHBoxLayout* nameLayout = new QHBoxLayout();
        nameLayout->setContentsMargins(1,1,1,1);
        QLabel* name = new QLabel(input.name + ":");
        name->setToolTip(input.desc);
        QLabel* icon = new QLabel();
        icon->setToolTip(input.typeName);
        icon->setPixmap(QPixmap::fromImage(input.icon));
        nameLayout->addWidget(icon);
        nameLayout->addWidget(name);
        QWidget* nameWidget = new QWidget();
        nameWidget->setLayout(nameLayout);
        if(mInterface.isConnected(portId))
            ui->portLayout->addRow(nameWidget, new QLabel("<em>" + tr("connected") + "</em>"));
        else
        {
             QVariant value = mInterface.getPortValue(portId);
             FilterInfoPortEditWidget* edit = new FilterInfoPortEditWidget(input, value);
             connect(edit, SIGNAL(newValue(QString,QVariant)), SLOT(onNewValue(QString,QVariant)));
             ui->portLayout->addRow(nameWidget, edit);
             mPortEdits[input.name] = edit;
        }
    }
}

void FilterInfoWidget::updatePortValue(int filterId, const QString &portId, const QVariant &var)
{
    if(filterId != mFilterId)
        return;
    FilterInfoPortEditWidget* edit = mPortEdits.value(portId, 0);
    if(edit)
        edit->setValue(var);
}

void FilterInfoWidget::addPortConnection(const PortId &, const PortId &target)
{
    if(target.filter != mFilterId)
        return;
    FilterInfoPortEditWidget* edit = mPortEdits.value(target.port, 0);
    if(edit)
        edit->setConnected(true);
}

void FilterInfoWidget::removePortConnection(const PortId &, const PortId &target)
{
    if(target.filter != mFilterId)
        return;
    FilterInfoPortEditWidget* edit = mPortEdits.value(target.port, 0);
    if(edit)
        edit->setConnected(false);
}

void FilterInfoWidget::onNewValue(const QString &name, const QVariant& value)
{
    emit newPortValue(mFilterId, name, value);
}
