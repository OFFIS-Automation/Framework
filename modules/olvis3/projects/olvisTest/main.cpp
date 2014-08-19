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

#include <QtCore/QCoreApplication>
#include <QDebug>
#include <OlvisCoreInterface.h>
#include <QDir>
#include <QVariant>

#include <ports/DirectoryPort.h>
#include <ports/FilePort.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Hello World\n";
    OlvisCoreInterface theInterface;
    OlvisInterface& interface = theInterface;
    interface.scanPluginDir("../../plugins/debug");
    int pid = interface.createProcessor("First");
#if 0
    int delay = interface.addFilter(pid, "minimum period");
    int dirIter = interface.addFilter(pid, "directory iterator");
    int imageIn = interface.addFilter(pid, "image input");
    int threshold = interface.addFilter(pid, "threshold");
    int makePath = interface.addFilter(pid, "path creator");
    int imageOut = interface.addFilter(pid, "image output");
    interface.connectFilter(dirIter, "outFile", imageIn, "inFile");
    interface.connectFilter(makePath, "outFile", imageOut, "inFile");
    interface.connectFilter(imageIn, "outImage", threshold, "inImage");
    interface.connectFilter(threshold, "outImage", imageOut, "inImage");
    interface.setPortValue(delay, "period", 1000);
    interface.setPortValue(dirIter, "dir", convert::variant(QDir("C:/Users/claas.diederichs/Pictures/test")));
    interface.setPortValue(makePath, "inDirectory", convert::variant(QDir("C:/Users/claas.diederichs/Pictures/test/out")));
#else
    int avi = interface.addFilter(pid, "avi input");
    int period = interface.addFilter(pid, "minimum fps");
    interface.connectFilter(avi, "outFps", period, "inFps");
    int makePath = interface.addFilter(pid, "path creator");
    int imageOut = interface.addFilter(pid, "image output");
    interface.connectFilter(makePath, "outFile", imageOut, "inFile");
    interface.connectFilter(avi, "outImage", imageOut, "inImage");
    interface.setPortValue(avi, "inFile", convert::variant(QFileInfo("C:/Users/claas.diederichs/Documents/neueLEDs.avi")));
    interface.setPortValue(makePath, "inDirectory", convert::variant(QDir("C:/Users/claas.diederichs/Pictures/test/out")));

#endif
    interface.start();

    return a.exec();

}
