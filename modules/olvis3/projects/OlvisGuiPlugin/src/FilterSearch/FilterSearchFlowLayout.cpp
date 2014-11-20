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

#include "FilterSearchFlowLayout.h"
#include <QWidget>


FilterSearchFlowLayout::FilterSearchFlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
 : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FilterSearchFlowLayout::FilterSearchFlowLayout(int margin, int hSpacing, int vSpacing)
 : m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FilterSearchFlowLayout::~FilterSearchFlowLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
    {
        delete item;
    }
}

void FilterSearchFlowLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

int FilterSearchFlowLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0)
    {
        return m_hSpace;
    }
    else
    {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int FilterSearchFlowLayout::verticalSpacing() const
{
    if (m_vSpace >= 0)
    {
        return m_vSpace;
    }
    else
    {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

int FilterSearchFlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem *FilterSearchFlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem *FilterSearchFlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
    {
        return itemList.takeAt(index);
    }
    else
    {
        return 0;
    }
}

Qt::Orientations FilterSearchFlowLayout::expandingDirections() const
{
    return 0;
}

bool FilterSearchFlowLayout::hasHeightForWidth() const
{
    return true;
}

int FilterSearchFlowLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    {
        return height;
    }
}

void FilterSearchFlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FilterSearchFlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize FilterSearchFlowLayout::minimumSize() const
{
    QSize size;
    QLayoutItem *item;
    foreach (item, itemList)
    {
        size = size.expandedTo(item->minimumSize());
    }

    size += QSize(2*margin(), 2*margin());
    return size;
}

int FilterSearchFlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    QLayoutItem *item;
    foreach (item, itemList)
    {
        QWidget *wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
        {
            spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        }
        int spaceY = verticalSpacing();
        if (spaceY == -1)
        {
            spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        }
        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
        {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
        {
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
        }

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}
int FilterSearchFlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent)
    {
        return -1;
    }
    else if (parent->isWidgetType())
    {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    }
    else
    {
        return static_cast<QLayout *>(parent)->spacing();
    }
}
