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

#ifndef FILTERINFOPORTEDITWIDGET_H
#define FILTERINFOPORTEDITWIDGET_H

#include <QWidget>
#include <QVariant>

#include <gui/PortEditInterface.h>

namespace Ui {
    class FilterInfoPortEditWidget;
}
struct PortInfo;

class FilterInfoPortEditWidget : public QWidget, public PortEditInterface::Callback
{
    Q_OBJECT

public:
    explicit FilterInfoPortEditWidget(const PortInfo& info, const QVariant& value, QWidget *parent = 0);
    ~FilterInfoPortEditWidget();
    void setValue(const QVariant& var);
    void setConnected(bool connected);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
private slots:
    void on_clear_clicked();

signals:
    void newValue(const QString& portId, const QVariant& variant);

private:
    void editFinished(const QVariant& variant);
    void editCanceled();
    bool mInEditMode, mConnected;
    PortEditInterface* mEdit;
    Ui::FilterInfoPortEditWidget *ui;
    QString mPortId;
};

#endif // FILTERINFOPORTEDITWIDGET_H
