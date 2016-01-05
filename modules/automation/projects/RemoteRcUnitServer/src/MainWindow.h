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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QObject* server, QWidget *parent = 0);
    ~MainWindow();
signals:
    void reconnect(QString host, int port);
    void openProject(const QString& projectFile);

private slots:
    void on_restart_clicked();
    void updateStatus(int state);
    void showListenError(QString error);
    void on_actionLoadProject_triggered();
    void onUnitsUpdated();

private:
    Ui::MainWindow *ui;
    QList<QDockWidget*> mRcWidgets;
};

#endif // MAINWINDOW_H
