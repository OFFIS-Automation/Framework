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

#ifndef PROCESSOROUTPUTSWIDGET_H
#define PROCESSOROUTPUTSWIDGET_H

#include <QFrame>

namespace Ui {
    class FilterWidget;
}
struct PortInfo;
class ProcessorOutputPortWidget;
class FilterPortWidget;

class ProcessorOutputsWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ProcessorOutputsWidget(QWidget *parent = 0);
    ~ProcessorOutputsWidget();
    void addTempPort(bool left, int filterId, const QString& portId);
    void clearTempPort();
    ProcessorOutputPortWidget* tempPort() { return mTemp; }
    ProcessorOutputPortWidget* addOutput(int filterId, const QString& portId, const QString& name);
    ProcessorOutputPortWidget* takeOutput(int filterId, const QString& portId);
    void renameOutput(int filterId, const QString& portId, const QString& name);
signals:
    void startConnect(FilterPortWidget* source);
    void startDisconnect(FilterPortWidget* target);
private:
    Ui::FilterWidget *ui;
    ProcessorOutputPortWidget* mTemp;
};

#endif // PROCESSOROUTPUTSWIDGET_H
