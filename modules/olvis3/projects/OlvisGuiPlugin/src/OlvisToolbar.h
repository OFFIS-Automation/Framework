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

#ifndef OLVISTOOLBAR_H
#define OLVISTOOLBAR_H

#include <QToolBar>

namespace Ui {
    class OlvisToolbar;
}

#include <QMenu>

class OlvisToolbar : public QToolBar
{
    Q_OBJECT

public:
    explicit OlvisToolbar(QWidget *parent = 0);
    ~OlvisToolbar();
    void initMenu(QMenu* olvisMenu);

signals:
    void reloadPlugins();
private slots:
    void on_actionCreatePlugin_triggered();
    void on_actionReloadPlugins_triggered();

    void onExecutionStarted();
    void onExecutionFinished();
private:
    Ui::OlvisToolbar *ui;
};

#endif // OLVISTOOLBAR_H
