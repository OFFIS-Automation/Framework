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


#include "ClientGui.h"
#include "ui_ClientGui.h"
#include <QMessageBox>

ClientGui::ClientGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientGui),
    mClient(&mSocket, &mSocket)
{
    ui->setupUi(this);
    ui->inputWidget->setEnabled(false);

}

ClientGui::~ClientGui()
{
    delete ui;
}


void ClientGui::on_logInfo_clicked()
{
    QString text = ui->input->text();
    ui->input->clear();
    mClient.logMessage(text);
    ui->input->setFocus();
}

void ClientGui::on_logWarning_clicked()
{
    QString text = ui->input->text();
    ui->input->clear();
    mClient.logError(text);
    ui->input->setFocus();
}

void ClientGui::on_echo_clicked()
{
    QString text = ui->input->text();
    ui->input->clear();
    QString echo = mClient.echo(text);
    ui->echoOutput->appendPlainText(echo);
}

void ClientGui::on_connect_clicked(bool checked)
{
    mSocket.connectToServer("qtRemoteSignalsTest");
    if(mSocket.waitForConnected(1))
    {
        connect(&mClient, SIGNAL(keepAlive()), ui->keepAlive, SLOT(stepUp()));
        mClient.initialize();
        ui->inputWidget->setEnabled(true);
        ui->connect->setEnabled(false);
    }
    else
    {
        QMessageBox::warning(this, "Could not connect", "Could not connect to server");
        ui->connect->setChecked(false);
    }
}
