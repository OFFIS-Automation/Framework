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

#ifndef TELECONTROLWIDGET_H
#define TELECONTROLWIDGET_H

#include <QDockWidget>
#include <QMap>
#include "TelecontrolAssignmentWidget.h"

class HapticInterface;

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
    void updateTelecontrolAssignment(const QString& deviceName, const QString& unitName);

    void activateGamepad(const QString& deviceName, const QString& unitName);
    void deactivateGamepadAll();
    void updateGamepadParameters(const QString& unitName, const QString& methodName, double sensitivity, QList<bool> inverts);

    void activateHaptic(const QString& deviceName, const QString& unitName);
    void deactivateHapticAll();
    void updateHapticParameters(const QString& unitName, const QString& methodName, double sensitivity, double forceScaling, QList<bool> inverts);

public slots:
    void clear();
    void onRcUnitsChanged(bool partialReload);
    void editButtonAssignment(const QString& unitName = QString());
    void openButtonAssignment(const QString& unitName);

private slots:
    void onTelecontrolAssignmentUpdate(const QString& deviceName, const QString& unitName);
    void onGamepadSwitchRequested(const QString &deviceName, const QString& unitName, bool down);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::TelecontrolWidget *ui;
    QStringList mTelecontrolUnits;
    QMap<int, QString> mUnitIndexes;
    QWidget* mHapticWidget;
    TelecontrolAssignmentWidget *mTelecontrolAssignmentWidget;
    QString mConfigFile;
};

#endif // TELECONTROLWIDGET_H
