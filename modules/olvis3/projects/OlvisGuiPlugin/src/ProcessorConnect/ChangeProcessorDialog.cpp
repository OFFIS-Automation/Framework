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

#include "ChangeProcessorDialog.h"
#include "ui_ChangeProcessorDialog.h"



ChangeProcessorDialog::ChangeProcessorDialog(int processorId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeProcessorDialog)
{
    ui->setupUi(this);
    const OlvisInterface& olvis = OlvisSingleton::instance();
    mInfo = olvis.getProcessor(processorId);
    connect(this, SIGNAL(createProcessor(QString)), &olvis, SLOT(createProcessor(QString)), Qt::DirectConnection);
    connect(this, SIGNAL(renameProcessor(int,QString)), &olvis, SLOT(renameProcessor(int,QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(connectProcessor(int,int)), &olvis, SLOT(connectProcessor(int,int)), Qt::QueuedConnection);
    connect(this, SIGNAL(disconnectProcessor(int,int)), &olvis, SLOT(disconnectProcessor(int,int)), Qt::QueuedConnection);
    connect(this, SIGNAL(setPriority(int,int)), &olvis, SLOT(setProcessorPriority(int,int)), Qt::QueuedConnection);
    connect(this, SIGNAL(setStartupMode(int,bool)), &olvis, SLOT(setProcessorStartupBehavior(int,bool)), Qt::QueuedConnection);
    if(!mInfo.isValid())
        mInfo.name = "newProcessor";
    ui->name->setText(mInfo.name);
    ui->priorityComboBox->setCurrentIndex(mInfo.priority);
    ui->startBehavior->addItem(tr("Normal start"), 0);
    ui->startBehavior->addItem(tr("Start paused"), -1);
    if(mInfo.pausedStartup)
        ui->startBehavior->setCurrentIndex(1);

    mTriggerId = -1;
    // is this processor triggered by another?
    foreach(ProcessingElementConnection conn, olvis.processingElementConnections())
    {
        if(conn.targetId == mInfo.id)
        {
            mTriggerId = conn.sourceId;
            break;
        }
    }

    foreach(const ProcessorInfo& info, olvis.getProcessors())
    {
        if(info.id == mInfo.id)
            continue;
        QString text = tr("Triggered by <%1>").arg(info.name);
        ui->startBehavior->addItem(text, info.id);
        if(info.id == mTriggerId)
            ui->startBehavior->setCurrentIndex(ui->startBehavior->count()-1);
    }

}

ChangeProcessorDialog::~ChangeProcessorDialog()
{
    delete ui;
}

void ChangeProcessorDialog::updateProcessor()
{
    const OlvisInterface& olvis = OlvisSingleton::instance();
    if(!mInfo.isValid())
    {
        emit createProcessor(ui->name->text());
        mInfo = olvis.getProcessor(ui->name->text());
    }
    else
        emit renameProcessor(mInfo.id, ui->name->text());

    int trigger = ui->startBehavior->itemData(ui->startBehavior->currentIndex()).toInt();
    bool pausedStartup = trigger == -1;
    if(pausedStartup != mInfo.pausedStartup)
        emit setStartupMode(mInfo.id, pausedStartup);
    QThread::Priority newPriority = static_cast<QThread::Priority>(ui->priorityComboBox->currentIndex());
    if(newPriority != mInfo.priority)
        emit setPriority(mInfo.id, newPriority);

    if(mTriggerId > 0 && mTriggerId != trigger)
        emit disconnectProcessor(mTriggerId, mInfo.id);
    if(trigger > 0 && mTriggerId != trigger)
        emit connectProcessor(trigger, mInfo.id);
}

