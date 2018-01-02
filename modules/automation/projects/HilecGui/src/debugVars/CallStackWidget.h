// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#ifndef CALLSTACKWIDGET_H
#define CALLSTACKWIDGET_H

#include <QDockWidget>
#include <QModelIndex>
namespace Ui {
    class CallStackWidget;
}

class QListWidgetItem;

class CallStackWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit CallStackWidget(QWidget *parent = 0);
    ~CallStackWidget();
signals:
    void focusLine(const QString& filename, int line);
public slots:
    void setProjectFile(const QString& projectFile);
private slots:
    void updateStack();

    void on_listWidget_itemActivated(QListWidgetItem *item);

private:
    Ui::CallStackWidget *ui;
    QString mBaseDir;
};

#endif // CALLSTACKWIDGET_H
