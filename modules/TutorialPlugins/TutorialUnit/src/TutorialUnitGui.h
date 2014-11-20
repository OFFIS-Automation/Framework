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

#ifndef TUTORIALUNITGUI_H
#define TUTORIALUNITGUI_H

#include <QWidget>


namespace Ui {
    class TutorialUnitGui;
}

class TutorialUnit;
class GraphicsView;

class TutorialUnitGui : public QWidget
{
    Q_OBJECT

public:
    explicit TutorialUnitGui(TutorialUnit* control, GraphicsView* scene);
    ~TutorialUnitGui();
signals:
    void connectClicked();
    void disconnectClicked();
    void resetSystem();
    void resetSystemRandom();

private slots:
    void updateStatus(bool connected);
    void onPositionUpdate(QPointF p, qreal rot);
    void on_connectButton_clicked(bool checked);
    void on_resetButton_clicked();
    void on_resetRandomButton_clicked();

private:
    TutorialUnit* mController;
    Ui::TutorialUnitGui *ui;
    GraphicsView* mScene;
};

#endif // TUTORIALUNITGUI_H
