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

#include "StartScreen.h"
#include "ui_StartScreen.h"
#include "QSettings"
#include "version.h"

StartScreen::StartScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);
    QStringList recent = QSettings().value("recentProjects").toStringList();
    ui->comboBox->addItems(recent);
    if(recent.empty())
        ui->recentBox->setEnabled(false);

    QString versionString = tr("Version: %1.%2 (Build %3, %4)").arg(Version::MAJOR).arg(Version::MINOR).arg(Version::BUILD).arg(Version::DATE);
    ui->versionLabel->setText(versionString);
}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::on_open_clicked()
{
    emit loadProject();
    close();
}

void StartScreen::on_recent_clicked()
{
    emit loadRecentProject(ui->comboBox->currentText());
    close();
}

void StartScreen::on_create_clicked()
{
    emit createProject();
    close();
}
