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


#ifndef TELECONTROLGAMEPADWIDGET_H
#define TELECONTROLGAMEPADWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <core/RcUnitHelp.h>

namespace Ui {
    class TelecontrolGamepadWidget;
}

class TelecontrolGamepadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TelecontrolGamepadWidget(const QString& unitName, const RcUnitHelp::TcMove& method, QWidget *parent = 0);
    ~TelecontrolGamepadWidget();

signals:
    void updateGamepadParameters(QString unit, QString method, double value, QList<bool> inverts);

private slots:
    void on_slider_sliderMoved(int position);
    void sendUpdate(int sliderPosition = -1);
    void changeSlider(const QString &deviceName, const QString& unit, bool increase);

private:

    Ui::TelecontrolGamepadWidget *ui;
    QString mUnit, mMethod;
    QList<QCheckBox*> mCheckboxes;
};

#endif // TELECONTROLGAMEPADWIDGET_H

