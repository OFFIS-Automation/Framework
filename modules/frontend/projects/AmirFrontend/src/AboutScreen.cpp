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

#include "AboutScreen.h"
#include "ui_AboutScreen.h"
#include <QDesktopServices>
#include <QUrl>
#include "version.h"
#include <QMessageBox>

AboutScreen::AboutScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutScreen)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QString versionString = QString("Version: %1.%2 (Build %3, %4)").arg(Version::MAJOR).arg(Version::MINOR).arg(Version::BUILD).arg(Version::DATE);
    ui->versionLabel->setText(versionString);
}

AboutScreen::~AboutScreen()
{
    delete ui;
}

void AboutScreen::on_closeButton_clicked()
{
    close();
}

void AboutScreen::on_supportButton_clicked()
{
    QString url = QString("http://www.automation.offis.de");
    QDesktopServices::openUrl(QUrl(url));
}
