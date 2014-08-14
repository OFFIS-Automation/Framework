# OFFIS Automation Framework
# Copyright (C) 2013-2014 OFFIS e.V.
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

TEMPLATE = lib

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins
TARGET = Hilec

win32*: DEFINES += _CRT_SECURE_NO_WARNINGS

INCLUDEPATH += ../../../frontend/include
INCLUDEPATH += ../../../olvis3/include
INCLUDEPATH += ../../include
INCLUDEPATH += ../RcUnits/src
INCLUDEPATH += ../remoteClient/src

# Python
win32*: INCLUDEPATH += $$(AmirDevDir)/python3/include
win32*: LIBS += -L$$(AmirDevDir)/python3/libs
unix:!macx:CONFIG += link_pkgconfig
unix:!macx:PKGCONFIG += python-3.2
unix:!macx:LIBS += `pkg-config python-3.2 --libs --cflags` # static library path

LIBS += -L$${targetDir}/plugins -lRcUnits -lRemoteClient
pylibs.path = $${DESTDIR}/hilec/python
win32*: pylibs.files += $$(AmirDevDir)/python3/Lib/*
unix:!macx: pylibs.files+= /usr/lib/python3.2/*
INSTALLS += pylibs

pyDlls.path = $${DESTDIR}
win32*: pyDlls.files += $$(AmirDevDir)/python3/bin/pytho*.dll
win32*: pyDlls.files += $$(AmirDevDir)/python3/bin/pytho*.exe
win32*: pyDlls.files += $$(AmirDevDir)/python3/bin/pytho*.pdb
unix:!macx :pyDlls.files += `which python3` #/usr/bin/python3
unix:!macx :pyDlls.files += `which python3.2` #/usr/bin/python3.2
unix:!macx :pyDlls.files += `which python3.2-dbg`  #/usr/bin/python3.2-dbg
INSTALLS       += pyDlls

amirlibs.path    = $${DESTDIR}/hilec/python
amirlibs.files  += python/*
INSTALLS       += amirlibs

lolecfiles.path    = $${DESTDIR}/hilec/include
lolecfiles.files  += ../../include/rc/*
INSTALLS       += lolecfiles

tcfiles.path    = $${DESTDIR}/hilec/telecontrol
tcfiles.files  += ../../include/telecontrol/*
INSTALLS       += tcfiles

tcConfig.path    = $${targetDir}
tcConfig.files  += ../../data/gamepads.ini
INSTALLS       += tcConfig


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
    ../../include/rc/types/HwRcUnit.h

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
    src/PythonLinter.cpp

OTHER_FILES += \
    python/util.py \
    python/rc.py \
    python/io.py \
    python/init.py \
    python/__init__.py \
    python/types.py \




