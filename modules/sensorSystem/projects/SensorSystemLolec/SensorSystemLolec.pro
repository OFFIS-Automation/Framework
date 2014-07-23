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

TARGET = SensorSystem
DESTDIR = $${targetDir}/plugins/lolecs

LIBS += -L$${targetDir}/plugins -lSensorSystem -lSensorTracer

INCLUDEPATH += ../SensorTracer/src
INCLUDEPATH += ../../../automation/include
INCLUDEPATH += ../../include

HEADERS += \
    src/SensorSystemLolec.h \
    src/SensorSystemLolecInterface.h

SOURCES += \
    src/SensorSystemLolec.cpp \
    src/SensorSystemLolecInterface.cpp
