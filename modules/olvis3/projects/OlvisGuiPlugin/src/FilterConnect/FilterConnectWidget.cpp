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

#include "FilterConnectWidget.h"
#include "ui_FilterConnectWidget.h"
#include "FilterWidget.h"
#include "FilterSortingArea.h"
#include "../ProcessorConnect/ChangeProcessorDialog.h"

#include <core/OlvisInterface.h>
#include <core/FilterInfo.h>
#include <core/ProcessorInfo.h>

#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QMessageBox>

FilterConnectWidget::FilterConnectWidget(const OlvisInterface &model, QWidget *parent) :
    QDockWidget(parent),
    mInterface(model),
    ui(new Ui::FilterConnectWidget)
{
    mCurrentProcessor = 0;
    ui->setupUi(this);
    connect(&mInterface,SIGNAL(filterCreated(FilterInfo,int)), SLOT(addFilter(FilterInfo, int)));
    connect(&mInterface,SIGNAL(filterDeleted(FilterInfo)), SLOT(removeFilter(FilterInfo)));
    connect(&mInterface, SIGNAL(filterMoved(FilterInfo,int)), SLOT(moveFilter(FilterInfo,int)));
    connect(&mInterface, SIGNAL(processorCreated(ProcessorInfo)), SLOT(addProcessor(ProcessorInfo)));
    connect(&mInterface, SIGNAL(processorDeleted(ProcessorInfo)), SLOT(removeProcessor(ProcessorInfo)));
    connect(&mInterface, SIGNAL(processorRenamed(ProcessorInfo,QString)), SLOT(renameProcessor(ProcessorInfo)));
    connect(&mInterface, SIGNAL(processorOutputCreated(PortId,QString)), SLOT(addProcessorOutput(PortId,QString)));
    connect(&mInterface, SIGNAL(processorOutputRenamed(PortId,QString)), SLOT(renameProcessorOutput(PortId,QString)));
    connect(&mInterface, SIGNAL(processorOutputDeleted(PortId)), SLOT(removeProcessorOutput(PortId)));
    connect(&mInterface, SIGNAL(filterConnected(PortId,PortId,int)), SLOT(addConnection(PortId, PortId,int)));
    connect(&mInterface, SIGNAL(filterDisconnected(PortId,PortId,int)), SLOT(removeConnection(PortId, PortId,int)));
    connect(&mInterface, SIGNAL(inputPortConnected(QString,PortId,int)), SLOT(addConnection(QString,PortId,int)));
    connect(&mInterface, SIGNAL(inputPortDisconnected(QString,PortId,int)), SLOT(removeConnection(QString,PortId,int)));
    connect(&mInterface, SIGNAL(processorsConnected(int,int)), SLOT(updateProcessorConnections(int, int)));
    connect(&mInterface, SIGNAL(processorsDisconnected(int,int)), SLOT(removeProcessorConnections(int, int)));
    connect(&mInterface, SIGNAL(processorOutputCreated(PortId,QString)), SLOT(updateProcessorConnections(PortId)));
    connect(&mInterface, SIGNAL(processorOutputDeleted(PortId)), SLOT(updateProcessorConnections(PortId)));
    connect(&mInterface, SIGNAL(processorOutputRenamed(PortId,QString)), SLOT(updateProcessorConnections(PortId)));

    connect(&mInterface,SIGNAL(makroFilterCreated(FilterInfo)), SLOT(addMakroFilter(FilterInfo)));
    connect(&mInterface, SIGNAL(makroInputCreated(QString,PortId,int)), SLOT(addMakroInput(QString,PortId,int)));
    connect(&mInterface, SIGNAL(makroInputRemoved(PortId,int)), SLOT(removeMakroInput(PortId,int)));
    connect(&mInterface,SIGNAL(makroFilterDeleted(int)), SLOT(removeMakroFilter(int)));

    connect(&mInterface, SIGNAL(filterExecutionError(QString,QString,int)), SLOT(showFilterError(QString,QString,int)));
    connect(&mInterface, SIGNAL(filterStartupError(QString,QString,int)), SLOT(showFilterError(QString,QString,int)));
    connect(&mInterface, SIGNAL(filterErrorCleared(QString,int)), SLOT(removeFilterError(QString,int)));

    connect(this, SIGNAL(newPortValue(int,QString,QVariant)), &mInterface, SLOT(setPortValue(int,QString,QVariant)), Qt::QueuedConnection);
    connect(this, SIGNAL(deleteFilterRequested(int)), &mInterface, SLOT(deleteFilter(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(addFilterRequested(int, QString)), &mInterface, SLOT(addFilter(int,QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(renameFilterRequested(int, QString)), &mInterface, SLOT(renameFilter(int,QString)), Qt::QueuedConnection);

    connect(this, SIGNAL(deleteProcessorRequested(int)), &mInterface, SLOT(deleteProcessor(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(deleteMakroFilterRequested(int)), &mInterface, SLOT(deleteMakroFilter(int)), Qt::QueuedConnection);
}

FilterConnectWidget::~FilterConnectWidget()
{
    delete ui;
}

void FilterConnectWidget::moveFilter(const FilterInfo &filter, int beforeFilter)
{
    mWidgets[filter.processorId]->moveWidget(filter, beforeFilter);
}

void FilterConnectWidget::addProcessorOutput(const PortId &id, const QString &name)
{
    FilterInfo filter = mInterface.getFilter(id.filter);
    mWidgets[filter.processorId]->addOutput(id, name);
}

void FilterConnectWidget::renameProcessorOutput(const PortId &id, const QString &name)
{
    FilterInfo filter = mInterface.getFilter(id.filter);
    mWidgets[filter.processorId]->renameOutput(id, name);
}

void FilterConnectWidget::removeProcessorOutput(const PortId &id)
{
    FilterInfo filter = mInterface.getFilter(id.filter);
    mWidgets[filter.processorId]->removeOutput(id);
}

void FilterConnectWidget::addConnection(const PortId &src, const PortId &target, int processorId)
{
    mWidgets[processorId]->addConnection(src, target);
}

void FilterConnectWidget::addConnection(const QString &src, const PortId &target, int processorId)
{
    mWidgets[processorId]->addConnection(src, target);
}

void FilterConnectWidget::removeConnection(const PortId &src, const PortId &target, int processorId)
{
    mWidgets[processorId]->removeConnection(src, target);
}

void FilterConnectWidget::removeConnection(const QString &src, const PortId &target, int processorId)
{
    mWidgets[processorId]->removeConnection(src, target);
}


void FilterConnectWidget::addFilter(const FilterInfo &filter, int beforeFilter)
{
    FilterWidget* w = new FilterWidget(filter);
    connect(w, SIGNAL(selected(int)), SIGNAL(filterSelected(int)));
    connect(w, SIGNAL(newPortValue(int,QString,QVariant)), SIGNAL(newPortValue(int,QString,QVariant)));
    connect(w, SIGNAL(deleteRequest(int)), SIGNAL(deleteFilterRequested(int)));
    connect(w, SIGNAL(renameRequested(int, QString)), SIGNAL(renameFilterRequested(int, QString)));
    mFilterWidgets[filter.name] = w;
    mWidgets[filter.processorId]->addWidget(w);
    mWidgets[filter.processorId]->moveWidget(filter, beforeFilter);

}

void FilterConnectWidget::removeFilter(const FilterInfo &filterInfo)
{
    QBoxLayout* layout = qobject_cast<QBoxLayout*>(mWidgets[filterInfo.processorId]->layout());
    FilterWidget* filter = mFilterWidgets.take(filterInfo.name);
    if(filter)
    {
        layout->removeWidget(filter);
        delete filter;
    }
}

void FilterConnectWidget::addMakroFilter(const FilterInfo &filter)
{
    FilterSortingArea* sortingArea = new FilterSortingArea(filter.id, mInterface, true);
    connect(sortingArea, SIGNAL(portSelected(int,QString)), SIGNAL(portSelected(int,QString)));
    sortingArea->hide();
    mWidgets[filter.id] =sortingArea;
    ui->sortLayout->addWidget(sortingArea);
    ui->comboBox->addItem(tr("MacroFilter: ") + filter.name, QVariant(filter.id));
    ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
}

void FilterConnectWidget::removeMakroFilter(int makroId)
{
    FilterSortingArea* area = mWidgets.take(makroId);
    if(!area)
        return;
    ui->sortLayout->removeWidget(area);
    for(int i= 0; i< ui->comboBox->count(); i++)
    {
        int id = ui->comboBox->itemData(i).toInt();
        if(id == makroId)
        {
            ui->comboBox->removeItem(i);
            break;
        }
    }
    delete area;
}

void FilterConnectWidget::addMakroInput(QString name, const PortId& portId, int makroId)
{
    FilterSortingArea* area = mWidgets.value(makroId, 0);
    if(area)
        area->addInput(name, portId);
}

void FilterConnectWidget::removeMakroInput(const PortId& portId, int makroId)
{
    FilterSortingArea* area = mWidgets.value(makroId, 0);
    if(area)
        area->removeInput(portId);
}


void FilterConnectWidget::requestAddFilter(const QString &uid)
{
    emit addFilterRequested(mCurrentProcessor, uid);
}

void FilterConnectWidget::addProcessor(const ProcessorInfo &processor)
{
    FilterSortingArea* sortingArea = new FilterSortingArea(processor.id, mInterface, false);
    connect(sortingArea, SIGNAL(portSelected(int,QString)), SIGNAL(portSelected(int,QString)));
    sortingArea->hide();
    mWidgets[processor.id] =sortingArea;
    ui->sortLayout->addWidget(sortingArea);
    ui->comboBox->addItem(processor.name, QVariant(processor.id));
    selectProcessor(processor.id);
}

void FilterConnectWidget::removeProcessor(const ProcessorInfo &processor)
{
    FilterSortingArea* area = mWidgets.take(processor.id);
    if(!area)
        return;
    ui->sortLayout->removeWidget(area);
    for(int i= 0; i< ui->comboBox->count(); i++)
    {
        int id = ui->comboBox->itemData(i).toInt();
        if(id == processor.id)
        {
            ui->comboBox->removeItem(i);
            break;
        }
    }
    delete area;
}

void FilterConnectWidget::renameProcessor(const ProcessorInfo &processor)
{
    for(int i= 0; i< ui->comboBox->count(); i++)
    {
        int id = ui->comboBox->itemData(i).toInt();
        if(id == processor.id)
        {
            ui->comboBox->setItemText(i, processor.name);
            break;
        }
    }
}

void FilterConnectWidget::selectProcessor(int id)
{
    for(int i=0; i<ui->comboBox->count(); i++)
    {
        if(ui->comboBox->itemData(i).toInt() == id)
        {
            ui->comboBox->setCurrentIndex(i);
            break;
        }
    }
}

void FilterConnectWidget::on_comboBox_currentIndexChanged(int index)
{
    FilterSortingArea* last = mWidgets.value(mCurrentProcessor, 0);
    if(last)
        last->hide();
    mCurrentProcessor = ui->comboBox->itemData(index).toInt();
    bool isProcessor = mInterface.getProcessor(mCurrentProcessor).isValid();
    ui->openSettingsButton->setVisible(isProcessor);
    FilterSortingArea* current = mWidgets.value(mCurrentProcessor, 0);
    if(current)
        current->show();
}

void FilterConnectWidget::on_deleteProcessoButton_clicked()
{
    // Get processor name
    ProcessorInfo info = mInterface.getProcessor(mCurrentProcessor);

    if(info.isValid())
    {
        // Delete the processor / ask the user for permission
        if(QMessageBox::question(0, tr("Delete processor"), tr("Do you really want to delete the processor <%1>?").arg(info.name),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
            return;
        // User really wants to delete => go on and delete
        emit deleteProcessorRequested(mCurrentProcessor);
        return;
    }
    FilterGroupInfo gInfo = mInterface.getMakroFilter(mCurrentProcessor);
    if(gInfo.isValid())
    {
        // Delete the makro filter / ask the user for permission
        if(QMessageBox::question(0, tr("Delete macro filter"), tr("Do you really want to delete the macro filter <%1>?").arg(gInfo.name),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
            return;
        // User really wants to delete => go on and delete
        emit deleteMakroFilterRequested(mCurrentProcessor);
        return;
    }
}

void FilterConnectWidget::on_openSettingsButton_clicked()
{
    // Open the change processor dialog with the current ID
    ChangeProcessorDialog dialog(mCurrentProcessor);
    if(dialog.exec())
        dialog.updateProcessor();
}

void FilterConnectWidget::startDrag(FilterWidget *widget, const QPoint& hotspot)
{
    FilterSortingArea* area = mWidgets.value(mCurrentProcessor, 0);
    if(area)
        area->startDrag(widget, hotspot);
}

void FilterConnectWidget::updateProcessorConnections(int source, int target)
{
    FilterSortingArea* area = mWidgets.value(target, 0);
    if(area)
        area->updateInputs(source);
}

void FilterConnectWidget::removeProcessorConnections(int, int target)
{
    FilterSortingArea* area = mWidgets.value(target, 0);
    if(area)
        area->removeInputs();
}

void FilterConnectWidget::updateProcessorConnections(const PortId &outputPort)
{
    FilterInfo filter = mInterface.getFilter(outputPort.filter);
    if(!filter.isValid())
        return;
    foreach(ProcessingElementConnection connection, mInterface.processingElementConnections())
    {
        if(connection.sourceId == filter.processorId && mWidgets.contains(connection.targetId))
            mWidgets[connection.targetId]->updateInputs(filter.processorId);
    }
}

void FilterConnectWidget::showFilterError(const QString &filter, const QString &errMsg, int /* processorId */)
{
    FilterWidget* widget  = mFilterWidgets.value(filter);
    if(widget)
        widget->showError(errMsg);
}

void FilterConnectWidget::removeFilterError(const QString &filter, int  /* processorId */)
{
    FilterWidget* widget  = mFilterWidgets.value(filter);
    if(widget)
        widget->clearError();
}
