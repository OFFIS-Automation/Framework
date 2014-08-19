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

#ifndef FILTERINFOWIDGET_H
#define FILTERINFOWIDGET_H

#include <QDockWidget>
#include <QMap>

namespace Ui {
    class FilterInfoWidget;
}
class QVariant;
class OlvisInterface;
class PluginContainer;
struct FilterInfo;
struct PortId;
class FilterInfoPortEditWidget;

class FilterInfoWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit FilterInfoWidget(const OlvisInterface& interface, QWidget *parent = 0);
    ~FilterInfoWidget();
public slots:
    void showFilter(const FilterInfo& info);
    void showFilter(int id);
    void onFilterRenamed(const FilterInfo& info);
    void updatePortValue(int filterId, const QString& portId, const QVariant& var);
    void addPortConnection(const PortId& src, const PortId& target);
    void removePortConnection(const PortId& src, const PortId& target);
private slots:
    void onNewValue(const QString& name, const QVariant& value);
signals:
    void newPortValue(int filterId, const QString& portId, const QVariant& value);
private:
    void updatePorts(const FilterInfo& info);
    void clearPorts();
    const OlvisInterface& mInterface;
    Ui::FilterInfoWidget *ui;
    int mFilterId;
    QMap<QString, FilterInfoPortEditWidget*> mPortEdits;

};

#endif // FILTERINFOWIDGET_H
