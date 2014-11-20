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

#ifndef FILTERCONNECTWIDGET_H
#define FILTERCONNECTWIDGET_H

#include <QDockWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

namespace Ui {
    class FilterConnectWidget;
}

class OlvisInterface;
struct FilterInfo;
struct ProcessorInfo;
struct PortId;
class FilterSortingArea;
class FilterWidget;

class FilterConnectWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit FilterConnectWidget(const OlvisInterface& model, QWidget *parent = 0);
    ~FilterConnectWidget();
    void startDrag(FilterWidget* widget, const QPoint& hotspot);
    void requestAddFilter(const QString& uid);
signals:
    void filterSelected(int filterId);
    void portSelected(int filterId, const QString& portId);
    void addFilterRequested(int processor, QString uid);
    void moveFilterRequested(int id, int beforeId);
    void connectRequested(int srcFilter, QString srcPort, int targetFilter, QString targetPort);
    void disconnectRequested(int targetFilter, QString targetPort);
    void deleteFilterRequested(int filterId);
    void renameFilterRequested(int filterId, const QString& newName);
    void newPortValue(int filterId, const QString& portId, const QVariant& value);
    void deleteProcessorRequested(int id);
    void deleteMakroFilterRequested(int id);
public slots:
    void addFilter(const FilterInfo& filter, int beforeFilter);
    void addMakroFilter(const FilterInfo& filter);
    void removeMakroFilter(int id);
    void addMakroInput(QString name, const PortId& portId, int makroId);
    void removeMakroInput(const PortId &portId, int makroId);
    void removeFilter(const FilterInfo& filter);
    void moveFilter(const FilterInfo& filter, int beforeFilter);
    void addProcessorOutput(const PortId& id, const QString& name);
    void renameProcessorOutput(const PortId& id, const QString& name);
    void removeProcessorOutput(const PortId& id);
    void addProcessor(const ProcessorInfo& processor);
    void removeProcessor(const ProcessorInfo& processor);
    void renameProcessor(const ProcessorInfo& processor);
    void selectProcessor(int id);
    void addConnection(const PortId& src, const PortId& target, int processorId);
    void addConnection(const QString& src, const PortId& target, int processorId);
    void removeConnection(const PortId& src, const PortId& target, int processorId);
    void removeConnection(const QString& src, const PortId& target, int processorId);
    void updateProcessorConnections(int source, int target);
    void removeProcessorConnections(int source, int target);
    void updateProcessorConnections(const PortId& outputPort);
    void showFilterError(const QString& filter, const QString& errMsg, int processorId);
    void removeFilterError(const QString& filter, int processorId);


private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_deleteProcessoButton_clicked();
    void on_openSettingsButton_clicked();

private:
    const OlvisInterface& mInterface;
    Ui::FilterConnectWidget *ui;
    int mCurrentProcessor;
    QMap<int, FilterSortingArea*> mWidgets;
    QMap<QString, FilterWidget*> mFilterWidgets;

};

#endif // FILTERCONNECTWIDGET_H
