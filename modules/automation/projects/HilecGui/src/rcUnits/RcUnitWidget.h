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

#ifndef RCUNITWIDGET_H
#define RCUNITWIDGET_H

#include <QDockWidget>

namespace Ui {
    class RcUnitWidget;
}

class RcUnitWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit RcUnitWidget(QWidget *parent = 0);
    ~RcUnitWidget();
    void setProjectFile(const QString& projectFile) { mProjectFile = projectFile; }
signals:
    void addPluginToProject();
    void projectFileChanged(QString);
public slots:
    void updateRcUnits();
    void enableContent();
    void disableContent();
    void clear();

private slots:
    void on_units_customContextMenuRequested(const QPoint &pos);

    void on_actionAddLolec_triggered();

    void on_actionRemoveLolec_triggered();

private:
    QString mProjectFile;
    Ui::RcUnitWidget *ui;
};

#endif // RCUNITWIDGET_H
