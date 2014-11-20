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

#ifndef FILTERSORTINGAREA_H
#define FILTERSORTINGAREA_H

#include <QWidget>
#include <QSpacerItem>
#include <QMap>

class OlvisInterface;
struct FilterInfo;
struct PortId;
class FilterPortWidget;
class FilterWidget;
class FilterOutputPortWidget;

namespace Ui
{
    class FilterSortingArea;
}
class FilterSortingArea : public QWidget
{
    Q_OBJECT
public:
    explicit FilterSortingArea(int processorId, const OlvisInterface& model, bool allowInputCreation, QWidget *parent = 0);
    virtual ~FilterSortingArea();

signals:
    void portSelected(int filterId, const QString& portId);
    void moveFilterRequested(int filterId, int beforePos);
    void addFilterRequested(int processorId, QString filterType, int beforePos);
    void connectRequested(int srcFilter, QString srcPort, int targetFilter, QString targetPort);
    void connectInputRequested(const QString& inputName, int targetFilter, const QString& targetPort);
    void disconnectRequested(int targetFilter, QString targetPort);
    void deleteProcessorOutputRequested(int sourceFilter, QString sourcePort);
    void addProcessorOutputRequested(int filterId, const QString& portId);

    void delayedUpdateReq();

public slots:
    void startDrag(FilterWidget* widget, const QPoint &hotspot);
    void addWidget(FilterWidget* widget);
    void moveWidget(const FilterInfo& info, int beforeId);
    void addConnection(const PortId& src, const PortId& target);
    void addConnection(const QString& src, const PortId& target);
    void removeConnection(const PortId& src, const PortId& target);
    void removeConnection(const QString& src, const PortId& target);
    void addOutput(const PortId& portId, const QString& name);
    void renameOutput(const PortId& portId, const QString& name);
    void removeOutput(const PortId& portId);
    void updateInputs(int sourceProcessorId);
    void addInput(QString name, const PortId& port);
    void removeInput(const PortId &id);
    void removeInputs();

private slots:
    void onStartConnect(FilterPortWidget* source);
    void onStartDisconnect(FilterPortWidget* target);

protected:
    int dragAnimation(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dropEvent(QDropEvent *event);
    //void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

    QWidget* mEmptyWidget;
    struct
    {
        QPoint start, end;
        bool valid;
        bool possible;
        bool hasWarning;
        FilterPortWidget* ignore;
        FilterPortWidget* source;
    } mCurrent;
    const OlvisInterface& mInterface;
    int mProcessorId;
    typedef QList<FilterPortWidget*> PortWidgetList;
    QMap<FilterPortWidget*, PortWidgetList> mConnections;
    Ui::FilterSortingArea* ui;

    struct LayerUsage
    {
        LayerUsage(int s, int e) : start(s), end(e){}
        int start, end;
    };
    typedef QList<LayerUsage> LayerUsageList;
    FilterWidget* mLastDragAnimationWidget;
    bool mAllowInputCreate;
};

#endif // FILTERSORTINGAREA_H
