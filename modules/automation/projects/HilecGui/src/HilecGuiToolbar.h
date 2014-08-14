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

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QMenu>

namespace Ui {
    class HilecGuiToolbar;
}

class HilecGuiToolbar : public QToolBar
{
    Q_OBJECT

public:
    explicit HilecGuiToolbar(QWidget *parent = 0);
    ~HilecGuiToolbar();
    void createMenu(QMenu* menu);

signals:
    void aboutToRunFile(QString);
    void runFile(QString);
    void showHelpWidget();
    void createGamepadMapping();
    void addRcServer();
public slots:
    void setCurrentFile(const QString& filename) { mFile = filename; }
    void setConfigFile(const QString& configFile) { mConfigFile = configFile; }
    void setEnabled(bool enabled);

private slots:
    void onScriptExecutionStarted();
    void onScriptExecutionFinished();
    void on_actionResume_triggered();

    void on_actionCreateRcUnit_triggered();

    void on_actionAddRcUnit_triggered();

    void on_actionRcUnitHelp_triggered();

    void on_actionCreateGamepadMapping_triggered();

    void on_actionAddRcServer_triggered();

private:
    Ui::HilecGuiToolbar *ui;
    QString mFile;
    QString mConfigFile;
    bool mRunning;
};

#endif // TOOLBAR_H
