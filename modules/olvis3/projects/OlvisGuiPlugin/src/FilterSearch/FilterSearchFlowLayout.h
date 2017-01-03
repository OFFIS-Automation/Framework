// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#ifndef FILTERSEARCHFLOWLAYOUT_H
#define FILTERSEARCHFLOWLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QWidgetItem>
#include <QStyle>

class FilterSearchFlowLayout : public QLayout
{
public:
    FilterSearchFlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    FilterSearchFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~FilterSearchFlowLayout();

    void addItem(QLayoutItem *item);
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const;
    bool hasHeightForWidth() const;
    int heightForWidth(int) const;
    int count() const;
    QLayoutItem *itemAt(int index) const;
    QSize minimumSize() const;
    void setGeometry(const QRect &rect);
    QSize sizeHint() const;
    QLayoutItem *takeAt(int index);

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
};

#endif // FILTERSEARCHFLOWLAYOUT_H
