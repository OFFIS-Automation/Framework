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

#include "MasterWindow.h"
#include "ui_MasterWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <QDate>

#include "version.h"
#include "newProjectWizard/CreateProjectDialog.h"

MasterWindow::MasterWindow(QWidget *parent) :
    MainWindow(parent, false),
    ui(new Ui::MasterWindow)
{
    mStarted = false;
    ui->setupUi(this);
    setCentralWidget(mCentral);
    QSettings settings;
    mRecentProjects = settings.value("recentProjects").toStringList();
    ui->actionRecent_projects->setMenu(new QMenu());
    mRecentMenu = ui->actionRecent_projects->menu();
    updateRecentProjects();
    ui->actionClose_project->setEnabled(false);
    ui->actionReload_project->setEnabled(false);

    // Start screen
    mStartScreen = new StartScreen();
    connect(this, SIGNAL(showStartScreen()), mStartScreen, SLOT(exec()), Qt::QueuedConnection);
    connect(mStartScreen, SIGNAL(createProject()), SLOT(on_actionNew_project_triggered()), Qt::QueuedConnection);
    connect(mStartScreen, SIGNAL(loadProject()), SLOT(on_actionOpen_project_triggered()), Qt::QueuedConnection);
    connect(mStartScreen, SIGNAL(loadRecentProject(QString)), SLOT(open(QString)), Qt::QueuedConnection);

    // About screen
    mAboutScreen = new AboutScreen();
    connect(this, SIGNAL(showAboutScreen()), mAboutScreen, SLOT(exec()), Qt::QueuedConnection);
}

MasterWindow::~MasterWindow()
{
    delete ui;
}

QMenu *MasterWindow::getMenu(QString name)
{
    QMenuBar* bar = menuBar();
    foreach(QAction* action, bar->actions()){
        if(action->text() == name && action->menu() != 0){
            return action->menu();
        }
    }
    QMenu* newMenu = new QMenu(name);
    bar->insertMenu(ui->menuHelp->menuAction(), newMenu);
    return newMenu;
}

void MasterWindow::on_actionOpen_project_triggered()
{
    QString defaultOpen = QDir::homePath();
    if(!mRecentProjects.empty())
        defaultOpen = mRecentProjects.first();

    QString filter = "OFFIS automation projects (*.oap);;Old projects (*.pro *.ogr)";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open project"), defaultOpen, filter);

    if(filename.isEmpty())
        return;
    if(!QFileInfo(filename).exists())
        return;

    // Convert old
    bool oldAutomationFile = filename.endsWith(".pro", Qt::CaseInsensitive);
    bool oldOlvisFile = filename.endsWith(".ogr", Qt::CaseInsensitive);
    if(oldAutomationFile || oldOlvisFile)
    {
        if (QMessageBox::information(this, tr("Update project"), tr("The project needs to be updated. Proceed?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
        {
            QFileInfo info(filename);
            QString newName = info.absolutePath() + "/" + info.baseName() + ".oap";
            if(oldAutomationFile)
                QFile::rename(filename,newName);
            QSettings settings(newName, QSettings::IniFormat);
            settings.setValue("version", 2.0);
            if(oldOlvisFile)
                settings.setValue("visionFile", info.fileName());
            filename = newName;
        }
    }
    open(filename);
}

void MasterWindow::on_actionNew_project_triggered()
{
    QString defaultOpen = QDir::homePath();
    if(!mRecentProjects.empty())
    {
        QDir dir = QFileInfo(mRecentProjects.first()).absoluteDir();
        dir.cdUp();
        defaultOpen = dir.absolutePath();
    }
    CreateProjectDialog dialog(defaultOpen, this);
    if(!dialog.exec())
        return;
    QString filename = dialog.getProjectFileName();
    if(filename.isEmpty())
        return;
    QSettings settings(filename, QSettings::IniFormat);
    settings.clear();
    settings.setValue("version", 2.0);
    settings.sync();
    open(filename);
}

void MasterWindow::updateRecentProjects(const QString &mostRecent)
{
    if(!mostRecent.isEmpty())
    {
        mRecentProjects.removeAll(mostRecent);
        if(QFileInfo(mostRecent).exists())
            mRecentProjects.prepend(mostRecent);
    }
    QSettings().setValue("recentProjects", mRecentProjects);
    foreach(QAction* action, mRecentMenu->actions())
    {
        mRecentMenu->removeAction(action);
        action->deleteLater();
    }
    foreach(QString project, mRecentProjects)
    {
        mRecentMenu->addAction(QIcon(":/img/loadProject.png"),project,this,SLOT(onOpenRecentTriggered()));
    }
    ui->actionRecent_projects->setEnabled(!mRecentProjects.empty());
}

void MasterWindow::onOpenRecentTriggered()
{
    QAction * action = qobject_cast<QAction*>(sender());
    if(action)
        open(action->text());
}

void MasterWindow::open(const QString &projectPath)
{
    // Close current project
    emit closeProject();

    // Check if project file exist => then reopen
    if(!QFileInfo(projectPath).exists())
    {
        QMessageBox::warning(this, tr("Project file not found"), tr("The project could not be loaded, the project file was not found"));
        updateRecentProjects(projectPath);
        return;
    }
    updateRecentProjects(projectPath);
    emit openProject(projectPath);

    ui->actionClose_project->setEnabled(true);
    ui->actionReload_project->setEnabled(true);
}

void MasterWindow::on_actionClose_project_triggered()
{
    emit closeProject();
    ui->actionReload_project->setEnabled(false);
    ui->actionClose_project->setEnabled(false);
}

void MasterWindow::on_actionReload_project_triggered()
{
    // Close project
    closeProject();
    // Open project
    if(!mRecentProjects.empty()){
        QString projectPath = mRecentProjects.first();
        openProject(projectPath);
    }
}

void MasterWindow::on_actionAbout_OFFIS_Automation_Toolbox_triggered()
{
    emit showAboutScreen();
}

void MasterWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MasterWindow::setVisible(bool visible)
{
    QMainWindow::setVisible(visible);
    if(visible && !mStarted)
    {
        mStarted = false;
        ui->menu_File->addSeparator();
        ui->menu_File->addAction(ui->actionExit);
        emit showStartScreen();
    }
}
