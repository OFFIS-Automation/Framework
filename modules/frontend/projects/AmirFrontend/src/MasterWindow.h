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

#ifndef MASTERWINDOW_H
#define MASTERWINDOW_H

#include "MainWindow.h"
#include "StartScreen.h"
#include "AboutScreen.h"

namespace Ui {
class MasterWindow;
}

class MasterWindow : public MainWindow
{
    Q_OBJECT
    
public:
    explicit MasterWindow(QWidget *parent = 0);
    ~MasterWindow();
    QMenu* getMenu(QString name);
signals:
    void openProject(const QString& projectPath);
    void closeProject();
    void showStartScreen();
    void showAboutScreen();
    void showHelpWidget();
public slots:
    virtual void setVisible(bool visible);

private slots:
    void onOpenRecentTriggered();
    void on_actionOpen_project_triggered();
    void on_actionNew_project_triggered();
    void on_actionClose_project_triggered();
    void open(const QString &projectPath);

    void on_actionAbout_OFFIS_Automation_Toolbox_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionReload_project_triggered();
    void on_actionScripting_help_triggered();

private:
    bool mStarted;
    void updateRecentProjects(const QString& mostRecent = QString());
    QStringList mRecentProjects;
    Ui::MasterWindow *ui;
    QMenu* mRecentMenu;
    StartScreen* mStartScreen;
    AboutScreen* mAboutScreen;
};

#endif // MASTERWINDOW_H
