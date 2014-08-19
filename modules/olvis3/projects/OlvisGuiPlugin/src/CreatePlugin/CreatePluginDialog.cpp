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

#include "CreatePluginDialog.h"
#include "ui_CreatePluginDialog.h"

#include <QFileDialog>
#include <QSettings>
#include <QDir>

#include <QFile>
#include <QPushButton>
#include <QCoreApplication>
#include <QTextStream>

CreatePluginDialog::CreatePluginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePluginDialog)
{
    ui->setupUi(this);
    mSourceDir = QCoreApplication::applicationDirPath() + "/plugins/olvis/templates/filter";
    QSettings settings;
    QString path = settings.value("olvisGuiPlugin/userPluginDir", QDir::homePath()).toString();
    QString pluginName = settings.value("olvisGuiPlugin/userPluginName", QString("CustomPlugin")).toString();
    QDir dir(path);
    ui->destination->setText(dir.canonicalPath());
    ui->pluginName->setText(pluginName);
    QStringList templates = QDir(mSourceDir + "/src").entryList(QStringList("*.h"), QDir::Files);
    foreach(QString tpl, templates)
    {
        tpl.chop(2);
        ui->templateSelect->addItem(tpl);
    }

    // Hide help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

CreatePluginDialog::~CreatePluginDialog()
{
    delete ui;
}

void CreatePluginDialog::on_chooseDestination_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), ui->destination->text());
    if(!dir.isEmpty())
        ui->destination->setText(dir);
}

void CreatePluginDialog::createPlugin()
{
    QString dirName = ui->destination->text();
    QString project = ui->pluginName->text();
    QString filter = ui->filterName->text();
    QSettings settings;
    settings.setValue("olvisGuiPlugin/userPluginDir", dirName);
    settings.setValue("olvisGuiPlugin/userPluginName", project);
    if(project.isEmpty() || filter.isEmpty() || dirName.isEmpty())
        return;

    QDir dir(dirName);
    dir.mkdir(project);
    dir.cd(project);
    dir.mkdir("src");
    QDir srcDir = dir;
    srcDir.cd("src");

    QString targetHeader = srcDir.absoluteFilePath(filter + ".h");
    QString targetSource = srcDir.absoluteFilePath(filter + ".cpp");

    QString source = ui->templateSelect->currentText();

    QString data = readFile(mSourceDir + "/src/" + source + ".h");
    data.replace(source, filter);
    data.replace(source.toUpper()+ "_H", filter.toUpper()+ "_H");
    writeFile(targetHeader, data);

    data = readFile(mSourceDir + "/src/" + source + ".cpp");
    data.replace(source, filter);
    writeFile(targetSource, data);

    QString targetProject = dir.absoluteFilePath(project+".pro");
    if(!QFileInfo(targetProject).exists())
    {
        data = readFile(mSourceDir + "/template.pro");
        data.replace("CustomPlugin", project);
        data.replace("CustomFilter", filter);
        writeFile(targetProject, data);

        data = readFile(mSourceDir + "/template.h");
        data.replace(QString("CustomPlugin").toUpper()+ "_H", project.toUpper()+ "_H");
        data.replace("CustomPlugin", project);
        writeFile(srcDir.absoluteFilePath(project + ".h"), data);

        data = readFile(mSourceDir + "/template.cpp");
        data.replace("CustomPlugin", project);
        writeFile(srcDir.absoluteFilePath(project + ".cpp"), data);
    }
    else
    {
        QFile f(targetProject);
        f.open(QFile::Append | QFile::Text);
        QTextStream stream(&f);
        stream << "HEADERS += src/" << filter << ".h" << endl;
        stream << "SOURCES += src/" << filter << ".cpp" << endl;
    }
}

QString CreatePluginDialog::readFile(const QString &filename)
{
    QFile f(filename);
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&f);
    return stream.readAll();
}

void CreatePluginDialog::writeFile(const QString &filename, const QString &data)
{
    QFile f(filename);
    f.open(QFile::WriteOnly | QFile::Text);
    QTextStream stream(&f);
    stream << data;
}

void CreatePluginDialog::on_destination_textChanged(const QString &)
{
    check();
}

void CreatePluginDialog::on_pluginName_textChanged(const QString &)
{
    check();
}

void CreatePluginDialog::on_filterName_textChanged(const QString &)
{
    check();
}

void CreatePluginDialog::check()
{
    QString filterName = ui->filterName->text();
    QString pluginName = ui->pluginName->text();
    QStringList errors;

    QRegExp rx("[A-Za-z][A-Za-z0-9_]+");
    if(!rx.exactMatch(pluginName))
        errors << tr("The plugin name must be a correct c++ class name");
    if(!rx.exactMatch(filterName))
        errors << tr("The filter name must be a correct c++ class name");

    // check if the filter name is different form the
    if(pluginName.compare(filterName, Qt::CaseInsensitive) == 0)
        errors << tr("The filter and the plugin can not have the same name");

    // check if the target filter exists
    QDir dir(ui->destination->text());
    QString path = dir.absoluteFilePath(ui->pluginName->text() + "/src/" + ui->filterName->text());
    if(QFileInfo(path + ".h").exists() || QFileInfo(path + ".cpp").exists())
    {
        errors << tr("A filter with the given name exists for the chosen plugin name in the chosen directory");
    }

    QPushButton* b = ui->buttonBox->button(QDialogButtonBox::Ok);
    if(b) b->setEnabled(errors.empty());
    ui->errorText->setText(errors.join("\n"));
}
