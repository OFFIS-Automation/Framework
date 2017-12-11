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

#ifndef TELECONTROLHAPTICWIDGET_H
#define TELECONTROLHAPTICWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <core/RcUnitHelp.h>

namespace Ui {
class TelecontrolHapticWidget;
}

class TelecontrolHapticWidget : public QWidget
{
    Q_OBJECT
    
public:

    explicit TelecontrolHapticWidget(QString unit, const RcUnitHelp::TcMove& method, QWidget *parent = 0);
    virtual ~TelecontrolHapticWidget();
    const static int numForceSteps = 10;
signals:
    void updateHapticParameters(QString unitName, QString method, double sensitivity, double forceScaling, QList<bool> inverts);

private slots:
    void sendHapticParamatersUpdate();
    void on_gainSlider_sliderMoved(int position);
    void on_forceSlider_sliderMoved(int position);

private:
    Ui::TelecontrolHapticWidget *ui;
    QString mUnit, mMethod;
    QList<QCheckBox*> mCheckboxes;
};

#endif // TELECONTROLHAPTICWIDGET_H
