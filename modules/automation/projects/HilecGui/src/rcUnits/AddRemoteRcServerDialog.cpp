// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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


#include "AddRemoteRcServerDialog.h"
#include "ui_AddRemoteRcServerDialog.h"

#include <QSettings>

AddRemoteRcServerDialog::AddRemoteRcServerDialog(const QString& projectFile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRemoteRcServerDialog),
    mProjectFile(projectFile)
{
    ui->setupUi(this);
    checkData();

    // Hide help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AddRemoteRcServerDialog::~AddRemoteRcServerDialog()
{
    delete ui;
}

void AddRemoteRcServerDialog::addToSettings()
{
    QSettings settings(mProjectFile, QSettings::IniFormat);
    int size = settings.beginReadArray("remoteServers");
    settings.endArray();
    settings.beginWriteArray("remoteServers");
    settings.setArrayIndex(size);
    settings.setValue("name", ui->name->text());
    settings.setValue("address", ui->address->text());
    settings.setValue("port", ui->port->value());
    settings.setValue("callTimeout", 5.0);
    settings.endArray();
}

void AddRemoteRcServerDialog::on_address_textChanged(const QString &)
{
    checkData();
}

void AddRemoteRcServerDialog::on_name_textChanged(const QString &)
{
    checkData();
}


void AddRemoteRcServerDialog::on_port_valueChanged(int)
{
    checkData();
}

void AddRemoteRcServerDialog::checkData()
{
    try{
        QString name = ui->name->text();
        QString address = ui->address->text();
        int port = ui->port->value();
        if(name.isEmpty())
            throw std::runtime_error("Please provide a name");
        if(address.isEmpty())
            throw std::runtime_error("Please provide a server address");

        QSettings settings(mProjectFile, QSettings::IniFormat);
        int size = settings.beginReadArray("remoteServers");
        for(int i=0;i<size;i++)
        {
            settings.setArrayIndex(i);
            if(settings.value("name").toString() == name)
                throw std::runtime_error("A remote server with that name already exists");
            if(settings.value("address").toString() == address &&
                    settings.value("port").toInt() == port)
                throw std::runtime_error("A remote server with that address and port already exists");
        }
        settings.endArray();
        ui->errorText->hide();
        ui->okayButton->setEnabled(true);
    }
    catch(const std::runtime_error& e)
    {
        ui->errorText->setText(e.what());
        ui->errorText->show();
        ui->okayButton->setEnabled(false);
    }
}
