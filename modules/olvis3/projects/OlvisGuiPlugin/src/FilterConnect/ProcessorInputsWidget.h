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

#ifndef PROCESSORINPUTSWIDGET_H
#define PROCESSORINPUTSWIDGET_H

#include <QFrame>
#include <QMap>

#include <core/ProcessorInfo.h>
#include <core/PortId.h>

namespace Ui {
    class FilterWidget;
}
class ProcessorInputPortWidget;
class FilterPortWidget;

class ProcessorInputsWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ProcessorInputsWidget(QWidget *parent = 0);
    virtual ~ProcessorInputsWidget();
    void setAllowPortCreation(bool allow);
    void addInput(const QString& name, int filterId, PortInfo info);
    void removeInput(const PortId& target);
    QString nameOf(const PortId& target);
public slots:
    QList<ProcessorInputPortWidget*> updatePorts(QList<FilterGroupPort> ports);
signals:
    void startConnect(FilterPortWidget* source);
    void startDisconnect(FilterPortWidget* target);
private:
    bool mAllowPortCreation;
    Ui::FilterWidget *ui;
    QMap<PortId, ProcessorInputPortWidget*> mPorts;
    ProcessorInputPortWidget *tmpPortLeft, *tmpPortRight;
};

#endif // PROCESSORINPUTSWIDGET_H
