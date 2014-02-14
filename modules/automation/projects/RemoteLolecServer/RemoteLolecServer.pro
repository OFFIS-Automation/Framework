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

#-------------------------------------------------
#
# Project created by QtCreator 2011-08-16T16:59:48
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}

TARGET = RemoteServer
TEMPLATE = app

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../frontend/projects/LogWidget
INCLUDEPATH += ../RcUnits/src

LIBS += -L$${targetDir}/plugins -lRcUnits
LIBS += -L$${targetDir} -lLogWidget

SIGNALDEFS = $${PWD}/../remoteClient/remoteSignals/RemoteRcUnit.signals
SIGNALTARGET = $${PWD}/src/remoteSignals/
SIGNALHEADER = remoteRcUnitServer.h
SIGNALSOURCE= RemoteRcUnitsServer.cpp
createSignalProxy.target = $${SIGNALTARGET}$${SIGNALHEADER}


win32-msvc*{
    createSignalProxy.commands = $${PWD}/../../../../subtree/qt-remote-signals/bin/qtRemoteSignals.exe $$SIGNALDEFS $$SIGNALTARGET --server-only
} else {
    createSignalProxy.commands = $${PWD}/../../../../subtree/qt-remote-signals/bin/qtRemoteSignals $$SIGNALDEFS $$SIGNALTARGET --server-only
}
createSignalProxy.depends = FORCE
QMAKE_EXTRA_TARGETS += createSignalProxy
PRE_TARGETDEPS += $${SIGNALTARGET}$${SIGNALHEADER}

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/Server.cpp \
    src/RcUnits.cpp \
    src/GamepadWrapper.cpp \
    src/remoteSignals/RemoteSignals.cpp \
    src/remoteSignals/RemoteRcUnitServerBase.cpp \
    src/remoteSignals/RemoteRcUnitServer.cpp

HEADERS  += src/MainWindow.h \
    src/Server.h \
    src/RcUnits.h \
    src/GamepadWrapper.h \
    src/remoteSignals/RemoteSignals.h \
    src/remoteSignals/RemoteRcUnitServerBase.h \
    src/remoteSignals/RemoteRcUnitServer.h

FORMS    += src/MainWindow.ui



