# OFFIS Automation Framework
# Copyright (C) 2013-2017 OFFIS e.V.
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
INCLUDEPATH += ../RcUnits/src

DEFINES += REMOTECLIENT_LIBRARY

TARGET = RemoteClient
TEMPLATE = lib
CONFIG += lib

LIBS += -L$${targetDir}/plugins -lRcUnits

SIGNALDEFS = $${PWD}/remoteSignals/RemoteRcUnit.signals
SIGNALTARGET = $${PWD}/src/remoteSignals/
SIGNALHEADER = remoteRcUnitClientBase.h
SIGNALSOURCE= RemoteRcUnitsClientBase.cpp
createSignalProxy.target = $${SIGNALTARGET}$${SIGNALHEADER}


win32-msvc*{
    createSignalProxy.commands = $${PWD}/../../../../subtree/qt-remote-signals/bin/qtRemoteSignals.exe $$SIGNALDEFS $$SIGNALTARGET --client-only
} else {
    createSignalProxy.commands = $${PWD}/../../../../subtree/qt-remote-signals/bin/qtRemoteSignals $$SIGNALDEFS $$SIGNALTARGET --client-only
}
createSignalProxy.depends = FORCE
createSignalProxy.variable_out = HEADERS
QMAKE_EXTRA_TARGETS += createSignalProxy
PRE_TARGETDEPS += $${SIGNALTARGET}$${SIGNALHEADER}

OTHER_FILES += \
    remoteSignals/remoteRcUnit.signals

HEADERS += \
    src/remoteSignals/RemoteSignals.h \
    src/remoteSignals/RemoteRcUnitClient.h \
    src/remoteSignals/RemoteRcUnitClientBase.h \
    src/RemoteRcUnits.h \
    src/RemoteRcUnit.h \
    src/RemoteClientGlobal.h \
    src/remoteSignals/RemoteSignalThreadContainer.h

SOURCES += \
    src/remoteSignals/RemoteSignals.cpp \
    src/remoteSignals/RemoteRcUnitClient.cpp \
    src/remoteSignals/RemoteRcUnitClientBase.cpp \
    src/RemoteRcUnits.cpp \
    src/RemoteRcUnit.cpp
