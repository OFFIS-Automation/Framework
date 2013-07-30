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


#include <QCoreApplication>
#include <QProcess>
#include "src/TestClient.h"

#include <QTemporaryFile>
#include <QFileInfo>
#include <QLocalSocket>
#include <QDateTime>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QProcess process;
    QString pipeName = "pythonExec_" + QString::number(QDateTime().currentDateTime().toTime_t(), 32);
    qDebug() << pipeName;
    QStringList arguments(pipeName);
    process.start(a.applicationDirPath() + "/PyExecLib.exe", arguments);
    process.waitForStarted();
    QLocalSocket socket;
    socket.connectToServer(pipeName);
    if(socket.waitForConnected(2000))
    {
        TestClient client(socket, socket);
        return a.exec();
    }
}
