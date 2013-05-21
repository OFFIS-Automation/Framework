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

#include "ConfigurationWidget.h"
#include "ui_ConfigurationWidget.h"
#include <QDebug>
#include <QMenu>
#include <QFileDialog>
#include <QSettings>
#include <QKeyEvent>

#include "../HilecSingleton.h"

ConfigurationWidget::ConfigurationWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ConfigurationWidget)
{
    ui->setupUi(this);
    ui->executionWidget->hide();
    connect(this, SIGNAL(runRequested(QString)), HilecSingleton::hilec(), SLOT(runFile(QString)), Qt::QueuedConnection);
    connect(ui->quitButton, SIGNAL(clicked()), HilecSingleton::hilec(), SLOT(quit()), Qt::QueuedConnection);
    connect(HilecSingleton::hilec(), SIGNAL(scriptExecutionStarted()), SLOT(disableContent()));
    connect(HilecSingleton::hilec(), SIGNAL(scriptExecutionFinished()), SLOT(enableContent()));
    connect(HilecSingleton::hilec(), SIGNAL(scriptExecutionStarted()),ui->executionWidget, SLOT(show()));
    connect(HilecSingleton::hilec(), SIGNAL(scriptExecutionFinished()),ui->executionWidget, SLOT(hide()));
}

ConfigurationWidget::~ConfigurationWidget()
{
    delete ui;
}


void ConfigurationWidget::addFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Add Configuration"), mBaseDir, tr("Source Files (*.py)"));
    if(fileName.isEmpty())
        return;
    QListWidgetItem* item = createElement(fileName);
    ui->listWidget->addItem(item);
    saveConfig();
    ui->listWidget->editItem(item);
}

void ConfigurationWidget::on_listWidget_fileDropped(const QString &file)
{
    QListWidgetItem* item = createElement(file);
    ui->listWidget->addItem(item);
    saveConfig();
    ui->listWidget->editItem(item);
}


QListWidgetItem* ConfigurationWidget::createElement(const QString& fileName)
{
    QString name = fileName;
    name.remove(mBaseDir);
    QListWidgetItem* item = new QListWidgetItem(QIcon(":/ProjectEditor/bug_go.png"), name);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setData(Qt::UserRole, fileName);
    item->setToolTip(name);
    return item;
}



void ConfigurationWidget::editConfig(QListWidgetItem *item)
{
    if(!item)
        item = ui->listWidget->currentItem();
    if(!item)
        return;
    ui->listWidget->editItem(item);
}


void ConfigurationWidget::deleteConfig(QListWidgetItem *item)
{
    if(!item)
        item = ui->listWidget->currentItem();
    if(!item)
        return;
    ui->listWidget->takeItem(ui->listWidget->row(item));
    delete item;
    saveConfig();
}

void ConfigurationWidget::runConfig(QListWidgetItem *item)
{
    if(!item)
        item = ui->listWidget->currentItem();
    if(!item)
        return;
    emit runRequested(item->data(Qt::UserRole).toString());
}

void ConfigurationWidget::setConfigFile(const QString& filename)
{
    if(!mCurrentConfig.isEmpty())
        saveConfig();
    ui->listWidget->clear();
    mCurrentConfig = filename;
    if(filename.isEmpty())
        return;
    mBaseDir = QFileInfo(filename).absoluteDir().canonicalPath();
    if(!mBaseDir.endsWith("/"))
        mBaseDir += "/";
    QSettings settings(filename, QSettings::IniFormat);

    int count = settings.beginReadArray("runConfigurations");
    for(int i=0; i<count; i++)
    {
        settings.setArrayIndex(i);
        QString name = settings.value("name").toString();
        QString file = settings.value("file").toString();
        QListWidgetItem* item = createElement(mBaseDir + file);
        item->setText(name);
        ui->listWidget->addItem(item);
    }
    settings.endArray();
}

void ConfigurationWidget::saveConfig()
{
    QSettings settings(mCurrentConfig, QSettings::IniFormat);
    settings.beginWriteArray("runConfigurations");
    for(int i=0; i<ui->listWidget->count(); i++)
    {
        settings.setArrayIndex(i);
        QListWidgetItem* item = ui->listWidget->item(i);
        settings.setValue("name", item->text());
        settings.setValue("file", item->data(Qt::UserRole).toString().remove(mBaseDir));
    }
    settings.endArray();
}

void ConfigurationWidget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    runConfig(item);
}

void ConfigurationWidget::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    QListWidgetItem* item = ui->listWidget->itemAt(pos);
    if(item)
    {
        menu.addAction(ui->actionRun);
        menu.addAction(ui->actionRename);
        menu.addAction(ui->actionDelete);

    } else
        menu.addAction(ui->actionAdd);
    menu.exec(mapToGlobal(pos));
}

void ConfigurationWidget::on_listWidget_itemChanged(QListWidgetItem *)
{
    saveConfig();
}

void ConfigurationWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete && event->modifiers() == Qt::NoModifier)
    {
        deleteConfig();
        event->accept();
    }
    else
        QDockWidget::keyPressEvent(event);
}

void ConfigurationWidget::on_actionRun_triggered()
{
    runConfig();
}

void ConfigurationWidget::on_actionDelete_triggered()
{
    deleteConfig();
}

void ConfigurationWidget::on_actionRename_triggered()
{
    editConfig();
}

void ConfigurationWidget::on_actionAdd_triggered()
{
    addFile();
}

void ConfigurationWidget::disableContent()
{
    ui->listWidget->setEnabled(false);
}

void ConfigurationWidget::enableContent()
{
    ui->listWidget->setEnabled(true);
}
