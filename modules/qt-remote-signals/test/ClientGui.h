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


#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <QDialog>
#include <QLocalSocket>
#include "TestDataClient.h"

namespace Ui {
class ClientGui;
}

class ClientGui : public QDialog
{
    Q_OBJECT
    
public:
    explicit ClientGui(QWidget *parent = 0);
    ~ClientGui();
    

private slots:
    void on_logInfo_clicked();

    void on_logWarning_clicked();

    void on_echo_clicked();

    void on_connect_clicked(bool checked);

private:
    Ui::ClientGui *ui;
    QLocalSocket mSocket;
    TestDataClient mClient;
};

#endif // CLIENTGUI_H
