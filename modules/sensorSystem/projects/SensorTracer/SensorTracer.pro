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
# Project created by QtCreator 2013-03-11T16:14:07
#
#-------------------------------------------------


TARGET = SensorTracer
TEMPLATE = lib

DEFINES += SENSORTRACER_LIBRARY

INCLUDEPATH += ../../include
include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins

LIBS += -L$${targetDir}/plugins -lSensorSystem

HEADERS += \
    src/SensorTracer_global.h \
    src/SensorTraceExport.h \
    src/SensorDataWriter.h

SOURCES += \
    src/SensorTraceExport.cpp \
    src/SensorDataWriter.cpp
