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

#ifndef PROCESSORSWIDGET_H
#define PROCESSORSWIDGET_H

#include <QDockWidget>
#include <QAction>
#include <QToolBox>
#include <QSpinBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QButtonGroup>
#include <QLineEdit>
#include <QGraphicsTextItem>
#include <QFont>
#include <QToolButton>
#include <QAbstractButton>
#include <QGraphicsView>
#include <QToolBar>

#include "DiagramScene.h"
#include <QXmlStreamWriter>
const int InsertTextButton = 10;

namespace Ui {
    class ProcessorsWidget;
}

/*
  This class represents a widget for visualizations of the processors and
  their connections. It contains a graphics scene
  (DiagramScene) and three buttons, one for each diagram item
  (processor, join, buffer).
  Also there are functions to save and restore the current state of the
  diagram scene.
  */
class ProcessorsWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ProcessorsWidget(QWidget *parent = 0);
    ~ProcessorsWidget();
    void saveCurrentState(QXmlStreamWriter& writer);
    void restoreState(QXmlStreamReader& reader);

signals:
    void processorSelected(int id);

private:
    Ui::ProcessorsWidget *ui;
    DiagramScene *scene;
};

#endif // PROCESSORSWIDGET_H
