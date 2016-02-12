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

#-------------------------------------------------
#
# Project created by QtCreator 2011-10-05T13:29:54
#
#-------------------------------------------------

QT       -= gui

TARGET = SensorSystem
TEMPLATE = lib

INCLUDEPATH += ../../include

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins

DEFINES += SENSORDATASYSTEM_LIBRARY

copyHeader.path    = $${DESTDIR}/sensorSystem/include
copyHeader.files  += ../../include/SensorDataSystem.h
copyHeader.files  += ../../include/Timer.h
INSTALLS       += copyHeader

copyLib.path    = $${DESTDIR}/sensorSystem/lib
copyLib.files  += $${DESTDIR}/$${TARGET}.lib
INSTALLS       += copyLib


HEADERS += \
    src/SensorSystem.h \
    src/Consumer.h \
    src/Provider.h \
    ../../include/SensorDataSystem.h \
    ../../include/Timer.h

SOURCES += \
    src/SensorSystemInterface.cpp \
    src/SensorSystem.cpp \
    src/Consumer.cpp \
    src/Provider.cpp \
    src/Timer.cpp
