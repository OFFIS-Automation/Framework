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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile inFile, outFile;
    inFile.open(stdin, QFile::ReadOnly);
    outFile.open(stdout, QFile::WriteOnly);
    PythonProcessServer server(inFile, outFile);
    return a.exec();
}
