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


INCLUDEPATH += ../../../frontend/include
INCLUDEPATH += ../../include
DESTDIR = $${targetDir}/plugins/hilec/python
TARGET = offisio
QMAKE_EXTENSION_SHLIB = pyd
TEMPLATE = lib

SIGNALDEFS = $${PWD}/signalProxy/PythonProcess.signals
SIGNALTARGET = $${PWD}/src/signalProxy/
SIGNALHEADER = PythonProcessServer.h
SIGNALSOURCE= PythonProcessServer.cpp
createSignalProxy.target = $${SIGNALTARGET}$${SIGNALHEADER}


win32-msvc*{
    createSignalProxy.commands = $${PWD}/../../../SignalProxy/bin/SignalProxy.exe $$SIGNALDEFS $$SIGNALTARGET --server-only
} else {
    createSignalProxy.commands = $${PWD}/../../../SignalProxy/bin/SignalProxy $$SIGNALDEFS $$SIGNALTARGET --server-only
}
createSignalProxy.depends = FORCE
QMAKE_EXTRA_TARGETS += createSignalProxy
PRE_TARGETDEPS += $${SIGNALTARGET}$${SIGNALHEADER}


#INCLUDEPATH+="C:/Python33/include"
#LIBS += -LC:/Python33/libs -lpython3

win32*: INCLUDEPATH += $$(AmirDevDir)/python3/include
win32*: LIBS += -L$$(AmirDevDir)/python3/libs


HEADERS += \
    src/PythonTypeConverter.h \
    src/PythonPlugin.h \
    src/PythonDebugger.h \
    src/UserRequestParser.h \
    src/UserRequestManager.h \
    signalProxy/PythonProcess.signals \
    src/signalProxy/PythonProcessServer.h \
    src/signalProxy/SignalProxy.h \
    src/PythonProcessSingleton.h \
    src/RcUnitInvoker.h \
    src/CallStackDecoder.h

SOURCES += \
    src/PythonTypeConverter.cpp \
    src/PythonPlugin.cpp \
    src/PythonDebugger.cpp \
    src/UserRequestParser.cpp \
    src/UserRequestManager.cpp \
    src/signalProxy/SignalProxy.cpp \
    src/signalProxy/PythonProcessServer.cpp \
    src/PythonProcessSingleton.cpp \
    src/RcUnitInvoker.cpp \
    src/CallStackDecoder.cpp
