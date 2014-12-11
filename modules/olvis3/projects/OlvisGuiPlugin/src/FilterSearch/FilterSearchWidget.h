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

#ifndef FILTERSEARCHWIDGET_H
#define FILTERSEARCHWIDGET_H

#include "FilterSearchTree.h"
#include "FilterSearchTreeItem.h"

#include <core/OlvisInterface.h>
#include <core/FilterTypeInfo.h>

#include <QDockWidget>
#include <QCheckBox>
#include <QList>
#include <QTreeWidgetItem>


namespace Ui {
    class FilterSearchWidget;
}
class FilterConnectWidget;
class OlvisInterface;

class FilterSearchWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit FilterSearchWidget(const OlvisInterface& model, FilterConnectWidget* dragTarget, QWidget *parent = 0);
    ~FilterSearchWidget();

signals:
    void createMakroFilter(QString name);

private:
    Ui::FilterSearchWidget *ui;
    const OlvisInterface& mInterface;
    QList<FilterSearchTreeItem *> mFilterList;
    QList<FilterTypeInfo> mFilterTypeList;
    QMap<QString, QCheckBox*> mTagList;

private slots:
    void createNodes();
    void filterSearch(QString search = QString());
    void emptyFilterSearch();
    void on_createMakroFilter_clicked();
    void on_createProcessor_clicked();
};

#endif // FILTERSEARCHWIDGET_H
