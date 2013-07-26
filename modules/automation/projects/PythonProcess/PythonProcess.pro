# OFFIS Automation Framework
# Copyright (C) 2013 OFFIS e.V.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http:#www.gnu.org/licenses/>.


QT += network

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins

INCLUDEPATH += ../../../frontend/include
INCLUDEPATH += ../../include

SIGNALDEFS = $${PWD}/signalProxy/PythonProcess.slots
createSignalProxy.target = $$PWD/src/signalProxy/PythonProcessServer.h


win32-msvc*{
    createSignalProxy.commands = $${PWD}/../../../SignalProxy/bin/SignalProxy.exe $$SIGNALDEFS $$PWD/src/signalProxy --server-only
} else {
    createSignalProxy.commands = $${PWD}/../../../SignalProxy/bin/SignalProxy $$SIGNALDEFS $$PWD/src/signalProxy --server-only
}
createSignalProxy.depends = FORCE
QMAKE_EXTRA_TARGETS += createSignalProxy
PRE_TARGETDEPS += $$PWD/src/signalProxy/PythonProcessServer.h


DEFINES += PYEXEC_LIBRARY

TARGET = PyExecLib

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/signalProxy/SignalProxy.cpp \
    src/signalProxy/PythonProcessServer.cpp \
    src/EchoService.cpp

HEADERS += \
    src/signalProxy/SignalProxy.h \
    src/signalProxy/PythonProcessServer.h \
    src/EchoService.h

OTHER_FILES += \
    signalProxy/PythonProcess.slots

