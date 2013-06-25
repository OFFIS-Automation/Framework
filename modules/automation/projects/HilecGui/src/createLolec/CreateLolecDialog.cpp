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

#include "CreateLolecDialog.h"
#include "ui_CreateLolecDialog.h"

#include <QFileDialog>
#include <QSettings>
#include <QDir>

#include <QFile>
#include <QPushButton>
#include <QCoreApplication>
#include <QTextStream>

CreateLolecDialog::CreateLolecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateLolecDialog)
{
    // Hide help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->setupUi(this);
    mSourceDir = QCoreApplication::applicationDirPath() + "/plugins/hilec/template";
    QSettings settings;
    QString path = settings.value("HilecGuiPlugin/userLolecDir").toString();
    QDir dir(path);
    ui->destination->setText(dir.canonicalPath());
}

CreateLolecDialog::~CreateLolecDialog()
{
    delete ui;
}

void CreateLolecDialog::on_chooseDestination_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), ui->destination->text());
    if(!dir.isEmpty())
    {
        ui->destination->setText(dir);
        ui->destination->setCursorPosition(dir.size());
    }
}

void CreateLolecDialog::createPlugin()
{
    QString dirName = ui->destination->text();
    QString project = ui->pluginName->text();
    QSettings settings;
    settings.setValue("HilecGuiPlugin/userLolecDir", dirName);

    if(project.isEmpty() || dirName.isEmpty())
        return;

    QDir dir(dirName);
    dir.mkdir(project);
    dir.cd(project);
    dir.mkdir("src");
    QStringList files;
    QString source = "CustomLolec";
    files << "src/CustomLolec.h" << "src/CustomLolec.cpp";
    files << "src/CustomLolecInterface.h" << "src/CustomLolecInterface.cpp";
    files << "src/CustomLolecGui.h" << "src/CustomLolecGui.cpp" << "src/CustomLolecGui.ui";
    files << "CustomLolec.pro";
    foreach(QString file, files)
    {
        QString data = readFile(mSourceDir + "/" + file);
        data.replace(source, project);
        data.replace(source.toUpper(), project.toUpper());
        QString targetFile = file;
        targetFile.replace(source, project);
        writeFile(dir.absoluteFilePath(targetFile), data);
    }
}

QString CreateLolecDialog::readFile(const QString &filename)
{
    QFile f(filename);
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&f);
    return stream.readAll();
}

void CreateLolecDialog::writeFile(const QString &filename, const QString &data)
{
    QFile f(filename);
    f.open(QFile::WriteOnly | QFile::Text);
    QTextStream stream(&f);
    stream << data;
}

void CreateLolecDialog::on_destination_textChanged(const QString &)
{
    check();
}

void CreateLolecDialog::on_pluginName_textChanged(const QString &)
{
    check();
}

void CreateLolecDialog::check()
{

    QDir dir(ui->destination->text());
    QString path = dir.absoluteFilePath(ui->pluginName->text());
    bool exists = QFileInfo(path).exists();
    QPushButton* b = ui->buttonBox->button(QDialogButtonBox::Ok);
    if(b) b->setEnabled(!exists);
    QString errStr;
    if(ui->pluginName->text().isEmpty())
        errStr = tr("Please enter a name for your Lolec.");
    else if(exists)
        errStr = tr("A Lolec with the given name exists in the chosen directory.");
    ui->errorText->setText(errStr);
}
