# OFFIS Automation Framework
# Copyright (C) 2013-2016 OFFIS e.V.
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

TEMPLATE = lib

VPATH += $$PWD

QT += network

win32*: DEFINES += _CRT_SECURE_NO_WARNINGS

INCLUDEPATH += $${PWD}/../../../frontend/include
INCLUDEPATH += $${PWD}/../../../olvis3/include
INCLUDEPATH += $${PWD}/../../include
INCLUDEPATH += $${PWD}/../RcUnits/src

LIBS += -L$${targetDir}/plugins -lRcUnits

!contains(DEFINES, NO_REMOTE_CLIENT) {
    INCLUDEPATH += $${PWD}/../remoteClient/src
    LIBS += -L$${targetDir}/plugins -lRemoteClient
}

# Python
win32*: INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/python3/include
win32*: LIBS += -L$$(OFFIS_DEVELOPMENT_ENVIRONMENT)/python3/libs
unix:!macx:CONFIG += link_pkgconfig
unix:!macx:PKGCONFIG += python-3.2
unix:!macx:LIBS += `pkg-config python-3.2 --libs --cflags` # static library path

pylibs.path = $${DESTDIR}/hilec/python
win32*: pylibs.files += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/python3/Lib/*
INSTALLS += pylibs

pyDlls.path = $${DESTDIR}
win32*: pyDlls.files += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/python3/bin/pytho*.dll
win32*: pyDlls.files += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/python3/bin/pytho*.exe
win32*: pyDlls.files += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/python3/bin/pytho*.pdb
INSTALLS       += pyDlls

amirlibs.path    = $${DESTDIR}/hilec/python
amirlibs.files  += $${PWD}/python/*
INSTALLS        += amirlibs

rcUnitfiles.path     = $${DESTDIR}/hilec/include
rcUnitfiles.files   += $${PWD}/../../include/rc/*
INSTALLS            += rcUnitfiles

tcfiles.path    = $${DESTDIR}/hilec/telecontrol
tcfiles.files  += $${PWD}/../../include/telecontrol/*
INSTALLS       += tcfiles

tcConfig.path    = $${targetDir}
tcConfig.files  += $${PWD}/../../data/gamepads.ini
INSTALLS        += tcConfig


HEADERS += \
    HilecPlugin.h \
    src/HilecCore.h \
    src/PythonInterpreter.h \
    src/UserRequestParser.h \
    src/RcUnits.h \
    src/PythonPlugin.h \
    src/RcUnitInvoker.h \
    src/OlvisSingleton.h \
    src/PythonDebugger.h \
    src/UserRequestManager.h \
    src/PythonTypeConverter.h \
    src/debugVariables/VariablesModel.h \
    src/debugVariables/VarTreeItem.h \
    src/debugVariables/DictVarItem.h \
    src/debugVariables/AbstractVarItem.h \
    src/debugVariables/ListVarItem.h \
    src/debugVariables/TupleVarItem.h \
    src/debugVariables/InstanceVarItem.h \
    src/CallStackDecoder.h \
    src/PythonLinter.h \
    ../../include/core/ScriptException.h \
    ../../include/core/UserRequest.h \
    ../../include/core/RcUnitHelp.h \
    ../../include/core/HilecInterface.h \
    ../../include/telecontrol/GamepadInterface.h \
    ../../include/telecontrol/HapticInterface.h \
    ../../include/telecontrol/TcConfig.h \
    ../../include/core/TelecontrolConfig.h \
    ../../include/rc/RcStruct.h \
    ../../include/rc/RcRepeatable.h \
    ../../include/rc/RcExceptions.h \
    ../../include/rc/RcUnitInterface.h \
    ../../include/rc/types/UserRcUnit.h \
    ../../include/rc/Pose2d.h \
    ../../include/rc/types/HwRcUnit.h \
    ../../include/rc/types/RobotRcUnit.h \
    ../../include/rc/types/RcUnitTypes.h \
    src/FlagCollectorRunnable.h

SOURCES += \
    HilecPlugin.cpp \
    src/HilecCore.cpp \
    src/PythonInterpreter.cpp \
    src/UserRequestParser.cpp \
    src/RcUnits.cpp \
    src/PythonPlugin.cpp \
    src/RcUnitInvoker.cpp \
    src/OlvisSingleton.cpp \
    src/PythonDebugger.cpp \
    src/UserRequestManager.cpp \
    src/PythonTypeConverter.cpp \
    src/debugVariables/VariablesModel.cpp \
    src/debugVariables/VarTreeItem.cpp \
    src/debugVariables/DictVarItem.cpp \
    src/debugVariables/AbstractVarItem.cpp \
    src/debugVariables/ListVarItem.cpp \
    src/debugVariables/TupleVarItem.cpp \
    src/debugVariables/InstanceVarItem.cpp \
    src/CallStackDecoder.cpp \
    src/PythonLinter.cpp \
    src/FlagCollectorRunnable.cpp

OTHER_FILES += \
    python/util.py \
    python/rc.py \
    python/io.py \
    python/init.py \
    python/__init__.py \
    python/types.py \
