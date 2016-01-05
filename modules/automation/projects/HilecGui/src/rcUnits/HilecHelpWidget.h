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

#ifndef HILECHELPWIDGET_H
#define HILECHELPWIDGET_H

#include <QDockWidget>
#include <QTreeWidgetItem>


namespace Ui {
    class HilecHelpWidget;
}



class HilecHelpWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit HilecHelpWidget(QWidget *parent = 0);
    ~HilecHelpWidget();

public slots:
    void updateRcUnits();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_expand_clicked();
    void on_collapse_clicked();
    void on_hideButton_clicked();
    void on_searchNext_clicked();
    void on_searchPrev_clicked();

    bool search(bool back = false);
    void clearSelection();

    void on_searchValue_textChanged(const QString &arg1);

private:
    void scanDocFolder();
    enum Type
    {
        RcUnitType = QTreeWidgetItem::UserType+1,
        FileType,
        FolderType
    };
    enum Role
    {
        FileRole = Qt::UserRole + 1
    };

    Ui::HilecHelpWidget *ui;
    QTreeWidgetItem* rcUnits;
    QMap<QString, QTreeWidgetItem*> mNavItems;
};

#endif // HILECHELPWIDGET_H
