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
// along with this program.  If not, see <http:#www.gnu.org/licenses/>.

#include <QCoreApplication>

#include <QDebug>
#include <QBuffer>
#include <QFile>

#include "signalProxy/PythonProcessServer.h"
#include <QLocalServer>
#include <QLocalSocket>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    if(args.size()<2) return 1;
    QString socketName = args[1];
    QLocalServer localServer;
    localServer.listen(socketName);
    if(localServer.waitForNewConnection(3000))
    {
        QLocalSocket* socket = localServer.nextPendingConnection();
        PythonProcessServer server(*socket, *socket);
        return a.exec();
    }
}
