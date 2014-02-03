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

#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QDialog>

namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT
    
public:
    explicit StartScreen(QWidget *parent = 0);
    ~StartScreen();

signals:
    void loadProject();
    void loadRecentProject(const QString& filename);
    void createProject();

private slots:
    void on_open_clicked();
    void on_recent_clicked();
    void on_create_clicked();

private:
    Ui::StartScreen *ui;
};

#endif // STARTSCREEN_H
