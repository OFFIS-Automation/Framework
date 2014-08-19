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

#ifndef DOCKWIDGETTITLE_H
#define DOCKWIDGETTITLE_H

#include <QFrame>
#include <QDockWidget>

class MainWindow;

namespace Ui {
    class DockWidgetTitle;
}

class DockWidgetTitle : public QFrame
{
    Q_OBJECT

public:
    explicit DockWidgetTitle(QWidget *parent = 0);
    ~DockWidgetTitle();
    void connectToDock(QDockWidget* dock);
    void setOtherWindow(MainWindow* window);
    virtual QSize minimumSizeHint();
private:
    Ui::DockWidgetTitle *ui;
    QDockWidget* mDock;
    MainWindow* mOther;

public slots:
    void updateFeatures(QDockWidget::DockWidgetFeatures features);

private slots:
    void on_switchWindow_clicked();
    void on_switchFloat_clicked();

    void on_makeCentral_clicked();
};

#endif // DOCKWIDGETTITLE_H
