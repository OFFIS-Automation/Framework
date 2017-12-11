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

#ifndef TELECONTROLASSIGNMENTWIDGET_H
#define TELECONTROLASSIGNMENTWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QMap>
#include "telecontrol/TcConfig.h"

namespace Ui {
class TelecontrolAssignmentWidget;
}

class TelecontrolAssignmentWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit TelecontrolAssignmentWidget(QWidget *parent = 0);
    ~TelecontrolAssignmentWidget();

public slots:
    void clear();
    void updateUnits(bool partialReload);
    void switchToUnit(QString unit);
    void onTelecontrolUpdated(const QString& deviceName, const QString& unitName, bool active);

private:
    Ui::TelecontrolAssignmentWidget *ui;
    QMap<int, QString> mUnitIndexes;
    static QStringList connexionControllerNames;
};

#endif // TELECONTROLASSIGNMENTWIDGET_H
