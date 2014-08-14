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


TEMPLATE = lib
CONFIG += dll

include(../../../properties/pathes.pro)

TARGET = OlvisLolec
DESTDIR = $${targetDir}/plugins/rcUnits

LIBS += -L$${targetDir}/plugins -lolvisCore

INCLUDEPATH += ../../../automation/include

INCLUDEPATH += ../../include
INCLUDEPATH += ../olvisCore/src

pythonFiles.path    = $${DESTDIR}/../hilec/python/rcunits
pythonFiles.files   += python/*
INSTALLS     += pythonFiles


HEADERS += \
    src/OlvisLolec.h \
    ../../../automation/include/rc/RcStruct.h \
    ../../../automation/include/rc/RcUnitInterface.h \
    ../../../automation/include/rc/RcRepeatable.h \
    ../../../automation/include/rc/RcExceptions.h \
    src/OlvisLolecInterface.h \
    src/portobserver.h \
    src/SignalWaiter.h \
    src/ProcessorOutputListener.h

SOURCES += \
    src/OlvisLolec.cpp \
    src/OlvisLolecInterface.cpp \
    src/portobserver.cpp \
    src/SignalWaiter.cpp \
    src/ProcessorOutputListener.cpp
