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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>
#include "interfaces/GuiPluginInterface.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, bool setupCentral = true);
    ~MainWindow();
    void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget);
    void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget, Qt::Orientation orientation);
    void addCentralDockWidget(Qt::DockWidgetArea fallbackArea, QDockWidget *dockwidget);
    void removeDockWidget(QDockWidget *dockwidget);
    void addToolBar(Qt::ToolBarArea area, QToolBar *toolbar);
    void addToolBar(QToolBar *toolbar);
    void setAlternative(MainWindow* window);
    void closeEvent(QCloseEvent *ev);
    void setCentralDockWidget(QDockWidget* dock);

    QDockWidget* centralDockWidget();
    QStringList saveDocks();
public slots:
    void restoreDocks(QStringList data, MainWindow* other, QStringList visibleObjects);

signals:
    void aboutToClose();
protected:
    QDockWidget* takeoverDock(const QString& name, MainWindow* other);
    void updateDockWidget(QDockWidget* d);
    QWidget* mCentral;
    QBoxLayout* mLayout;
    MainWindow* mOther;
};

#endif // MAINWINDOW_H
