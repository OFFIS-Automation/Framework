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

#include "AddRcUnitWizard.h"
#include "ui_AddRcUnitWizard.h"

#include <QFileDialog>
#include "../HilecSingleton.h"
#include <QSettings>
#include <QDebug>
#include <rc/RcUnitInterface.h>

#include <QPluginLoader>

AddRcUnitWizard::AddRcUnitWizard(const QString &projectFile, QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AddRcUnitWizard)
{

    mProjectFile = projectFile;
    ui->setupUi(this);
    ui->selectPage->init(ui);
    ui->definePage->init(ui);
    ui->nameError->hide();
    ui->fileWarning->hide();
    QDir rcUnitDir = QCoreApplication::applicationDirPath() + "/plugins/rcUnits";
    QStringList files = rcUnitDir.entryList(QStringList("*.dll") << "*.dynlib" << "*.so", QDir::Files, QDir::Name);
    foreach(QString file, files)
    {
        file = file.left(file.lastIndexOf("."));
        ui->rcUnitSelect->addItem(file);
        ui->rcUnitSelect->setCurrentIndex(-1);
    }

    // Hide help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(this, SIGNAL(projectFileChanged(QString)), HilecSingleton::hilec(), SLOT(loadConfig(QString)), Qt::QueuedConnection);
}

AddRcUnitWizard::~AddRcUnitWizard()
{
    delete ui;
}

void AddRcUnitWizard::addPlugin()
{
    if(!mRcUnit)
        return;
    if(mRcUnit->createConfig(ui->configFile->text(), parentWidget()))
    {
        QSettings settings(mProjectFile, QSettings::IniFormat);
        int size = settings.beginReadArray("hilecConfig");
        settings.endArray();
        settings.beginWriteArray("hilecConfig");
        settings.setArrayIndex(size);
        settings.setValue("name", ui->name->text());
        settings.setValue("type", ui->rcUnitSelect->currentText());
        if(mRcUnit->needsConfigFile())
        {
            QString configFile = ui->configFile->text();
            configFile = QFileInfo(mProjectFile).absoluteDir().relativeFilePath(configFile);
            settings.setValue("configFile", configFile);
        }
        settings.endArray();
        settings.sync();
        emit projectFileChanged(mProjectFile);
    }
}

void AddRcUnitWizard::on_selectFileButton_clicked()
{
    QString dir = QFileInfo(mProjectFile).absolutePath();
    QString name = QFileDialog::getSaveFileName(this, tr("Select config file"), dir, "*." +mFileExtension, 0, QFileDialog::DontConfirmOverwrite);
    if(!name.isEmpty())
        ui->configFile->setText(name);
}

void AddRcUnitWizard::on_rcUnitSelect_currentIndexChanged(const QString &type)
{
    setField("file", QString());
    ui->defaultName->clear();
    ui->description->clear();
    QString found;
    QStringList names;
    names << type + ".dll" << type + ".dylib" << type + ".so";
    QString rcUnitDir = QCoreApplication::applicationDirPath() + "/plugins/rcUnits";
    foreach(QString name, names)
    {
        QString path = rcUnitDir + "/" + name;
        if(QFileInfo(path).exists())
        {
            found = path;
            break;
        }
    }
    if(found.isEmpty())
        return;
    QString currentDir = QDir::current().absolutePath();
    QDir::setCurrent(QCoreApplication::applicationDirPath() + "/plugins");
    QPluginLoader loader(found);
    mRcUnit = 0;
    QObject* obj = loader.instance();
    if(!obj)
    {
        QString error = tr("The selected plugin could not be loaded: %1").arg(loader.errorString());
        ui->description->setTextColor(Qt::red);
        ui->description->setText(error);
    }
    else
    {
        mRcUnit = qobject_cast<RcUnitInterface*>(obj);
        if(!mRcUnit)
        {
            QString error = tr("The selected plugin could not be loaded: %1").arg(tr("The selected module is not a valid plugin. Maybe an old version?"));
            ui->description->setTextColor(Qt::red);
            ui->description->setText(error);
        }
        else
        {
            ui->description->setTextColor(Qt::black);
            ui->description->setText(mRcUnit->description());
            ui->defaultName->setText(mRcUnit->name());
            ui->fileBox->setVisible(mRcUnit->needsConfigFile());
            if(!mRcUnit->needsConfigFile())
                setField("file", "none");
            mFileExtension = mRcUnit->configFileExtension();
        }
    }
}

void AddRcUnitWizard::on_name_textChanged(const QString &arg1)
{
    QSettings settings(mProjectFile, QSettings::IniFormat);
    int size = settings.beginReadArray("hilecConfig");
    for(int i=0; i< size; i++)
    {
        settings.setArrayIndex(i);
        if(settings.value("name").toString() == arg1)
        {
            ui->definePage->setHasError(true);
            ui->nameError->show();
            return;
        }
    }
    ui->nameError->hide();
    ui->definePage->setHasError(arg1.isEmpty());
}

void AddRcUnitWizard::on_configFile_textChanged(const QString &arg1)
{
    ui->fileWarning->hide();
    if(arg1.isEmpty())
        return;
    QString filePath = QFileInfo(mProjectFile).absoluteDir().relativeFilePath(arg1);
    QSettings settings(mProjectFile, QSettings::IniFormat);
    int size = settings.beginReadArray("hilecConfig");

    for(int i=0; i< size; i++)
    {
        settings.setArrayIndex(i);
        if(settings.value("configFile").toString() == filePath)
        {
            ui->fileWarning->show();
            return;
        }
    }

}
