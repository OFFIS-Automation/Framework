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

#ifndef TELECONTROLWIDGET_H
#define TELECONTROLWIDGET_H

#include <QDockWidget>
#include <QMap>
#include "TelecontrolAssignmentWidget.h"

namespace Ui {
    class TelecontrolWidget;
}

class TelecontrolWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit TelecontrolWidget(QWidget *parent = 0);
    ~TelecontrolWidget();
    void setConfigFile(const QString& configFile) { mConfigFile = configFile; }
signals:
    void activateTelecontrol(const QString& unitName);
    void deactivateTelecontrol();
    void updateTelecontrol(const QString& unitName, const QString& methodName, double sensitivity, QList<bool> inverts);
    void activateHaptic(const QString& unitName);
    void deactivateHaptic();
    void updateHaptic(const QString& unitName, double sensitivity, double forceFactor);

public slots:
    void clear();
    void updateUnits(bool partialReload);
    void onTelecontrolUpdated(bool active, const QString& activeUnit);
    void onHapticUpdated(bool active, const QString& activeUnit);

private slots:
    void on_gamepadTabWidget_currentChanged(int index);
    void on_hapticTabWidget_currentChanged(int index);
    void on_openButtonAssignment_clicked(QString unit);
    void editButtonAssignment(const QString& unit);

private:
    static const int sHapticIndexOffset = 1024;
    Ui::TelecontrolWidget *ui;
    QMap<int, QString> mUnitIndexes;
    bool mInUpdate;
    QWidget* mHapticWidget;
    TelecontrolAssignmentWidget *mTelecontrolAssignmentWidget;
    QString mConfigFile;
};

#endif // TELECONTROLWIDGET_H
