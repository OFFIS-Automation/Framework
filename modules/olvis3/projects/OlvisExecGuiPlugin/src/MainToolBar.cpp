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

#include "MainToolBar.h"
#include "ui_MainToolBar.h"
#include "dialogs/ExportTraceDialog.h"
#include "dialogs/OlvisOptionsGui.h"

#include <core/OlvisInterface.h>
#include <core/FilterInfo.h>
#include <core/ProcessorInfo.h>

#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolTip>
#include <QProgressBar>
#include <QMenu>

MainToolBar::MainToolBar(const OlvisInterface &model, QWidget *parent) :
    QToolBar(parent),
    ui(new Ui::MainToolBar),
    mInterface(model)
{
    ui->setupUi(this);
    mRunActionGroup = new QActionGroup(this);
    mRunActionGroup->addAction(ui->actionRun);
    mRunActionGroup->addAction(ui->actionPause);
    mRunActionGroup->addAction(ui->actionStop);
    mRunActionGroup->addAction(ui->actionStepSingle);
    mRunActionGroup->addAction(ui->actionStep);

    mDisplayActionGroup = new QActionGroup(this);
    QList<int> displaysChoices;
    displaysChoices << 1 << 2 << 4 << 6 << 9 << 12 << 16;
    int defaultChoice = QSettings().value("OlvisExecGui/maxDisplays", 4).toInt();
    foreach(int choice, displaysChoices)
    {
        QAction* action = mImageDisplayMenu.addAction(QString::number(choice), this, SLOT(onNumDisplaysChanged()));
        action->setCheckable(true);
        mDisplayActionGroup->addAction(action);
        if(choice == defaultChoice)
            action->setChecked(true);
    }
    ui->actionImageDisplays->setMenu(&mImageDisplayMenu);

    ui->actionStop->setChecked(true);
    ui->actionProblems->setVisible(false);
    connect(&mInterface, SIGNAL(portValueChanged(int, QString,QVariant)), SLOT(check()));
    connect(&mInterface, SIGNAL(filterConnected(PortId,PortId,int)), SLOT(check()));
    connect(&mInterface, SIGNAL(filterDisconnected(PortId,PortId,int)), SLOT(check()));
    connect(&mInterface,SIGNAL(processorsDisconnected(int,int)), SLOT(check()));
    connect(&mInterface,SIGNAL(inputPortConnected(QString,PortId,int)), SLOT(check()));
    connect(&mInterface,SIGNAL(inputPortDisconnected(QString,PortId,int)), SLOT(check()));
    connect(&mInterface, SIGNAL(filterCreated(FilterInfo,int)), SLOT(check()));
    connect(&mInterface, SIGNAL(filterDeleted(FilterInfo)), SLOT(check()));
    connect(&mInterface, SIGNAL(processorCreated(ProcessorInfo)), SLOT(check()));
    connect(&mInterface, SIGNAL(processorDeleted(ProcessorInfo)), SLOT(check()));
    connect(&mInterface, SIGNAL(cleared()), SLOT(check()));
    connect(&mInterface, SIGNAL(executionFinished()), SLOT(onExecutionFinished()));
    connect(&mInterface, SIGNAL(executionStarted()), SLOT(onExecutionStarted()));
    connect(&mInterface, SIGNAL(makroInputCreated(QString,PortId,int)), SLOT(check()));
    connect(&mInterface, SIGNAL(makroInputRemoved(PortId,int)), SLOT(check()));

    // connect signals to olvis interface always QUEUED, to be sure gui has finished all repaints and animations
    connect(this, SIGNAL(startRequested()),& mInterface, SLOT(start()), Qt::QueuedConnection);
    connect(this, SIGNAL(stopRequested()), &mInterface, SLOT(stop()), Qt::QueuedConnection);
    connect(this, SIGNAL(pauseRequested()), &mInterface, SLOT(pause()), Qt::QueuedConnection);
    connect(this, SIGNAL(stepRequested(bool)), &mInterface, SLOT(step(bool)), Qt::QueuedConnection);
    connect(this, SIGNAL(setTraceEnabled(bool)), &mInterface, SLOT(setTracingEnabled(bool)), Qt::QueuedConnection);
    onExecutionFinished();
    check();
}

void MainToolBar::initMenu(QMenu* fileMenu, QMenu* olvisMenu)
{
    if(fileMenu->actions().size() > 0)
        fileMenu->addSeparator();

    if(olvisMenu->actions().size() > 0)
        olvisMenu->addSeparator();
    olvisMenu->addAction(ui->actionImageDisplays);
    olvisMenu->addAction(ui->actionRun);
    olvisMenu->addAction(ui->actionStop);
    olvisMenu->addAction(ui->actionPause);
    olvisMenu->addSeparator();
    olvisMenu->addAction(ui->actionOptions);
    olvisMenu->addAction(ui->actionEnableTracing);
    olvisMenu->addAction(ui->actionExportTrace);


}

MainToolBar::~MainToolBar()
{
    delete ui;
}

bool MainToolBar::check()
{
    QList<PortId> unconnected;
    bool valid = mInterface.check(unconnected);
    bool empty = mInterface.isEmpty();
    ui->actionProblems->setVisible(!unconnected.isEmpty());
    ui->actionAllOkay->setVisible(unconnected.isEmpty());
    mRunActionGroup->setEnabled(valid && !empty);
    if(!unconnected.isEmpty())
    {
        QString tooltip = tr("The following input ports are not connected and have no default value:\n");
        QListIterator<PortId> ports(unconnected);
        while(ports.hasNext())
        {
            const PortId& port = ports.next();
            FilterInfo filter = mInterface.getFilter(port.filter);
            ProcessorInfo proc = mInterface.getProcessor(filter.processorId);
            QString name = proc.name + "/" + filter.name + "/" + port.port;
            tooltip += "\n" + name;
        }
        ui->actionProblems->setToolTip(tooltip);
    }
    return valid;
}
void MainToolBar::onExecutionFinished()
{
    mRunActionGroup->setEnabled(true);
    ui->actionStop->setChecked(true);
    ui->actionStepSingle->setEnabled(false);
    ui->actionStep->setEnabled(false);
    ui->actionRestore->setEnabled(true);
}

void MainToolBar::onExecutionStarted()
{
    ui->actionRestore->setEnabled(false);
    ui->actionStepSingle->setEnabled(true);
    ui->actionStep->setEnabled(true);
    if(ui->actionStop->isChecked())
        ui->actionRun->setChecked(true);
}

void MainToolBar::on_actionRun_triggered()
{
    emit startRequested();
}


void MainToolBar::on_actionPause_triggered()
{
    emit pauseRequested();
}

void MainToolBar::on_actionStep_triggered()
{
    ui->actionPause->setChecked(true);
    emit stepRequested(false);
}

void MainToolBar::on_actionStepSingle_triggered()
{
    ui->actionPause->setChecked(true);
    emit stepRequested(true);

}

void MainToolBar::on_actionStop_triggered()
{
    if(mInterface.isRunning())
    {
        mRunActionGroup->setEnabled(false);
        emit stopRequested();
    }
}

void MainToolBar::on_actionProblems_triggered()
{
    check();
    QMessageBox::information(this, tr("Steps to perform"), ui->actionProblems->toolTip());
}


struct TraceElement
{
    double timestamp;
    int event;
    int name;
    int value;
    int id;
};

void MainToolBar::on_actionExportTrace_triggered()
{
    QString dir = QSettings().value("olvisExecGuiPlugin/traceExport").toString();
    QString name = QFileDialog::getSaveFileName(this, tr("Select file"), dir, tr("Comma seperated values (*.csv)"));
    if(name.isEmpty())
        return;
    QSettings().setValue("olvisExecGuiPlugin/traceExport", name);
    QFile outFile(name);
    outFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&outFile);

    QMap<int, QString> translation;

    QFile  translateFile(QCoreApplication::applicationDirPath() + "/trace.txt");
    translateFile.open(QFile::ReadOnly);
    QTextStream stream(&translateFile);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        int pos = line.indexOf("=");
        if(pos < 1)
            continue;
        int id = line.mid(0,pos).toInt();
        QString name = line.mid(pos+1).trimmed();
        translation[id] = name;
    }

    QFile dataFile(QCoreApplication::applicationDirPath() + "/trace.bin");
    if(!dataFile.open(QFile::ReadOnly))
        return;

    ExportTraceDialog dialog(this);
    dialog.show();
    QApplication::processEvents();
    int progress = 0;
    unsigned int step = dataFile.size() / (sizeof(TraceElement)*100);
    unsigned int count = 0;
    TraceElement trace;
    while(!dataFile.atEnd())
    {
        dataFile.read((char*)&trace, sizeof(TraceElement));
        out << trace.timestamp << ";" << translation[trace.name] << ";" << translation[trace.event] << ";" << translation[trace.value] << ";" << trace.id << endl;
        if(++count > step)
        {
            dialog.setValue(++progress);
            count = 0;
            QApplication::processEvents();
        }
    }
}


void MainToolBar::onNumDisplaysChanged()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
        return;
    bool success = false;
    int number = action->text().toInt(&success);
    if(success)
        emit setNumDisplays(number);
}


void MainToolBar::on_actionRestore_triggered()
{
    if(QMessageBox::warning(parentWidget(), tr("Restore vision configuration"), tr("Restore the saved vision configuration? Changes will get lost."),
                            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        emit restoreRequested();
    }
}

void MainToolBar::on_actionSave_triggered()
{
    emit saveRequested();
}

void MainToolBar::on_actionOptions_triggered()
{
    OlvisOptionsGui pluginConfig;
    pluginConfig.exec();
}


void MainToolBar::on_actionEnableTracing_triggered(bool checked)
{
    emit setTraceEnabled(checked);
}
