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

#include <QApplication>
#include "MainWindow.h"
#include "Server.h"
#include <QDebug>
#include <QSettings>

class Application : public QApplication
{
public:
    Application(int &argc, char* argv[]) : QApplication(argc, argv){}
    bool notify(QObject* receiver, QEvent* e) {
        try {
            return QApplication::notify(receiver, e);
        }   catch(const std::exception& err) {
            qCritical("%s", err.what());
            return false;
        }
    }
};

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    a.setOrganizationName("OFFIS");
    a.setApplicationName("RemoteLolecServer");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    Server server;
    QThread serverThread;
    serverThread.start();
    server.moveToThread(&serverThread);
    MainWindow w(&server);
    w.show();

    return a.exec();
}
