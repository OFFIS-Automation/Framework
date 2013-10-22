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

TEMPLATE = lib

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins
TARGET = Hilec

win32*: DEFINES += _CRT_SECURE_NO_WARNINGS

INCLUDEPATH += ../../../frontend/include
INCLUDEPATH += ../../../olvis3/include
INCLUDEPATH += ../../include
INCLUDEPATH += ../RcUnits/src

SIGNALDEFS = $${PWD}/../PythonPlugin/remoteSignals/PythonProcess.signals
SIGNALTARGET = $${PWD}/src/signalProxy/
SIGNALHEADER = PythonProcessClient.h
SIGNALSOURCE= PythonProcessClient.cpp
createSignalProxy.target = $${SIGNALTARGET}$${SIGNALHEADER}

win32-msvc*{
    createSignalProxy.commands = $${PWD}/../../../qt-remote-signals/bin/qtRemoteSignals.exe $$SIGNALDEFS $$SIGNALTARGET --client-only
} else {
    createSignalProxy.commands = $${PWD}/../../../qt-remote-signals/bin/qtRemoteSignals $$SIGNALDEFS $$SIGNALTARGET --client-only
}
createSignalProxy.depends = FORCE
QMAKE_EXTRA_TARGETS += createSignalProxy
PRE_TARGETDEPS += $${SIGNALTARGET}$${SIGNALHEADER}


LIBS += -L$${targetDir}/plugins -lRcUnits

pylibs.path    = $${DESTDIR}/hilec/python
win32*: pylibs.files += $$(AmirDevDir)/python3/Lib/*
unix:!macx: pylibs.files += /usr/lib/python3.2/*

INSTALLS       += pylibs

pyDlls.path = $${DESTDIR}
win32*: pyDlls.files += $$(AmirDevDir)/python3/bin/pytho*.dll
win32*: pyDlls.files += $$(AmirDevDir)/python3/bin/pytho*.exe
win32*: pyDlls.files += $$(AmirDevDir)/python3/bin/pytho*.pdb
unix:!macx :pyDlls.files += /usr/bin/python3
unix:!macx :pyDlls.files += /usr/bin/python3.2
unix:!macx :pyDlls.files += /usr/bin/python3.2-dbg

INSTALLS       += pyDlls

amirlibs.path    = $${DESTDIR}/hilec/python
amirlibs.files  += python/*
INSTALLS       += amirlibs

lolecfiles.path    = $${DESTDIR}/hilec/include
lolecfiles.files  += ../../include/lolecs/*
INSTALLS       += lolecfiles

tcfiles.path    = $${DESTDIR}/hilec/telecontrol
tcfiles.files  += ../../include/telecontrol/*
INSTALLS       += tcfiles

tcConfig.path    = $${targetDir}
tcConfig.files  += ../../data/gamepads.ini
INSTALLS       += tcConfig


HEADERS += \
    HilecPlugin.h \
    ../../include/core/HilecInterface.h \
    src/HilecCore.h \
    src/PythonInterpreter.h \
    ../../include/core/UserRequest.h \
    ../../include/core/ScriptException.h \
    src/RcUnits.h \
    ../../include/core/RcUnitHelp.h \
    src/OlvisSingleton.h \
    ../../include/lolecs/LolecInterface.h \
    ../../include/lolecs/RcStruct.h \
    ../../include/lolecs/RcRepeatable.h \
    ../../include/lolecs/RcExceptions.h \
    ../../include/lolecs/LolecInterface.h \
    src/RemoteLolec.h \
    src/RemoteRcUnit.h \
    src/PythonLinter.h \
    ../../include/lolecs/Pose2d.h \
    ../../include/telecontrol/GamepadInterface.h \
    ../../include/telecontrol/HapticInterface.h \
    ../../include/telecontrol/TcConfig.h \
    src/signalProxy/PythonProcessClient.h \
    src/PythonProcessControl.h \
    src/signalProxy/RemoteSignals.h

SOURCES += \
    HilecPlugin.cpp \
    src/HilecCore.cpp \
    src/PythonInterpreter.cpp \
    src/RcUnits.cpp \
    src/OlvisSingleton.cpp \
    src/RemoteLolec.cpp \
    src/RemoteRcUnit.cpp \
    src/PythonLinter.cpp \
    src/signalProxy/PythonProcessClient.cpp \
    src/PythonProcessControl.cpp \
    src/signalProxy/RemoteSignals.cpp

OTHER_FILES += \
    python/util.py \
    python/rc.py \
    python/io.py \
    python/init.py \
    python/__init__.py \
    python/types.py \




