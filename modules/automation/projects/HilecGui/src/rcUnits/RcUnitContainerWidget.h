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

#ifndef RCUNITCONTAINERWIDGET_H
#define RCUNITCONTAINERWIDGET_H

#include <QDockWidget>
#include <QBoxLayout>
#include <QSplitter>
#include "RcUnitFlagWidget.h"

namespace Ui {
    class RcUnitContainerWidget;
}

class RcUnitContainerWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit RcUnitContainerWidget(QWidget *parent = 0);
    ~RcUnitContainerWidget();

private slots:
    void updateRcUnits(bool partial);
    void unitFlagsUpdated(const QString& unitName, const QVariantList &flags);
    void onDockLocationChanged(const Qt::DockWidgetArea &area);

private:
    Ui::RcUnitContainerWidget *ui;
    QSplitter* splitter;
    Qt::Orientation mOrientation;
    QMap<QString, RcUnitFlagWidget*> mFlagWidgets;
};


#endif // RCUNITCONTAINERWIDGET_H
