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

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QNetworkInterface>
#include <QSettings>
#include <QFileDialog>
#include <QDockWidget>
#include <LogWindow.h>

#include "RcUnits.h"

MainWindow::MainWindow(QObject* server, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusConnected->setVisible(false);
    addDockWidget(Qt::BottomDockWidgetArea, new LogWindow());
    ui->errorString->hide();
    connect(this, SIGNAL(reconnect(QString,int)), server, SLOT(restart(QString, int)));
    connect(server, SIGNAL(stateChanged(int)), SLOT(updateStatus(int)));
    connect(server, SIGNAL(listenError(QString)), SLOT(showListenError(QString)));
    connect(this, SIGNAL(openProject(QString)), server, SLOT(loadProject(QString)));

    connect(RcUnits::instance(), SIGNAL(unitsUpdated()), SLOT(onUnitsUpdated()));
    QSettings settings;
    ui->port->setValue(settings.value("port", 4040).toInt());
    QString host = settings.value("host", "All").toString();
    foreach(const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces())
    {
        QNetworkInterface::InterfaceFlags flags = networkInterface.flags();
        if(networkInterface.isValid() && flags & QNetworkInterface::IsRunning && flags & QNetworkInterface::IsUp)
        {
            foreach(const QNetworkAddressEntry& entry, networkInterface.addressEntries())
            {
                QHostAddress address = entry.ip();
                if(address.protocol() == QAbstractSocket::IPv4Protocol)
                {
                    QString name = address.toString();
                    ui->host->addItem(address.toString());
                    if(name == host)
                        ui->host->setCurrentIndex(ui->host->count()-1);
                }
            }
        }
    }
    QString file = settings.value("configFile").toString();
    if(!file.isEmpty())
        emit openProject(file);
    ui->restart->click();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_restart_clicked()
{
    emit reconnect(ui->host->currentText(), ui->port->value());
}

void MainWindow::updateStatus(int state)
{
    ui->errorString->hide();

    switch(state)
    {
        case QAbstractSocket::ListeningState:
        case QAbstractSocket::UnconnectedState:
            ui->statusListen->setVisible(true);
            ui->statusConnected->setVisible(false);
            break;
        case QAbstractSocket::ConnectedState:
            ui->statusListen->setVisible(false);
            ui->statusConnected->setVisible(true);
            break;
    }
}

void MainWindow::showListenError(QString error)
{
    ui->errorString->setText(error);
    ui->errorString->show();
}

void MainWindow::on_actionLoadProject_triggered()
{
    QSettings settings;
    QString currentDir = settings.value("configFile").toString();
    QString project = QFileDialog::getOpenFileName(this, tr("Select configuration"), currentDir, tr("Project Files (*oap)"));
    if(project.isEmpty())
        return;
    settings.setValue("configFile", project);
    foreach(QDockWidget*dock,  mRcWidgets)
    {
        removeDockWidget(dock);
        delete dock;
    }
    mRcWidgets.clear();
    emit openProject(project);
}

void MainWindow::onUnitsUpdated()
{
    foreach(QString unitName, RcUnits::instance()->unitNames())
    {
        /*
         * TODO
        QWidget* w = RcUnits::instance()->rcUnitGui(unitName);
        if(w)
        {
            QDockWidget* dock = new QDockWidget("RcUnit: " +unitName, this);
            dock->setWidget(w);
            addDockWidget(Qt::TopDockWidgetArea, dock);
            mRcWidgets << dock;
        }
        */
    }
}
