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

#ifndef SCRIPTERRORWIDGET_H
#define SCRIPTERRORWIDGET_H

#include <QMap>
#include <QDockWidget>
#include <core/ScriptException.h>
#include <QTreeWidgetItem>

namespace Ui {
    class ScriptErrorWidget;
}

class ScriptErrorWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ScriptErrorWidget(QWidget *parent = 0);
    ~ScriptErrorWidget();
    void updateItems();

signals:
    void focusLine(const QString& file, int line);
public slots:
    void updateCompileError(const ScriptCompileInfo& info);
    void selectProblem(const QString& file, int line);
    void setProjectFile(const QString& path);
    void clear();
private slots:
    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_enableErrors_clicked(bool checked);
    void on_enableWarnings_clicked(bool checked);
    void on_enableInfos_clicked(bool checked);

private:
    Ui::ScriptErrorWidget *ui;
    QMap<int, QIcon> mErrorIcons;
    typedef QList<QTreeWidgetItem*> ListWidgetItems;
    QMap<QString, ListWidgetItems> mItems;
    QMap<int, bool> mModeEnabled;
    QString mBaseDir;
};

#endif // SCRIPTERRORWIDGET_H
