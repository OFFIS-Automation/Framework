// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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


#ifndef TUTORIALGUI_H
#define TUTORIALGUI_H

#include <QWidget>

namespace Ui {
class TutorialGui;
}
class TutorialUnit;

class TutorialGui : public QWidget
{
    Q_OBJECT

public:
    explicit TutorialGui(TutorialUnit* control);
    ~TutorialGui();

signals:
    void resetSystem();
    void resetSystemRandom();

private slots:
    void on_resetButton_clicked();
    void on_randomizeButton_clicked();

private:
    TutorialUnit* mController;
    Ui::TutorialGui *ui;
};

#endif // TUTORIALGUI_H
