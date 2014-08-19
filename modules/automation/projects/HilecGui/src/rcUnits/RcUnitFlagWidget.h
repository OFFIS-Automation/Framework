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


#ifndef RCUNITFLAGWIDGET_H
#define RCUNITFLAGWIDGET_H

#include <QLineEdit>
#include <QWidget>

#include <core/RcUnitHelp.h>

namespace Ui {
class RcUnitFlagWidget;
}

class RcUnitFlagWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RcUnitFlagWidget(const RcUnitHelp& help);
    ~RcUnitFlagWidget();
    void updateFlags(const QVariantList& flags);
    void unitStatusChanged(bool acquired);

signals:
    void stop(const QString& unitName);
    void acquire(const QString& unitName);
    void release(const QString& unitName);

private slots:
    void on_connectButton_clicked(bool checked);
    void on_stopButton_clicked();
    void on_groupBox_clicked(bool checked);
    void on_settingsButton_clicked(bool checked);

protected:
    RcUnitHelp mHelp;
    QList<QLineEdit*> mLineEdits;
    QByteArray mGeometry;
    QWidget* mDialog;
private:
    Ui::RcUnitFlagWidget *ui;
};

#endif // RCUNITFLAGWIDGET_H
