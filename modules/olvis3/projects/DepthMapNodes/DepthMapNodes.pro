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


TEMPLATE = lib
CONFIG+=dll
DEFINES += DEFAULTPORTS_LIBRARY
INCLUDEPATH += ../../include


include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins/olvisPlugins
TARGET = ContourNodes
LIBS += -L$${PWD}/lib -L$${targetDir}/plugins -lolvisFilter -lolvisPorts  -lolvisPlugin

INCLUDEPATH += ../../../sensorSystem/include
LIBS += -L$${targetDir}/plugins/ -lSensorSystem

include(../../../properties/opencv.pro)

HEADERS += \
    src/DepthMap2Image.h

SOURCES += \
    src/DepthMap2Image.cpp \
    src/main.cpp
HEADERS += src/DepthMapWriter.h
SOURCES += src/DepthMapWriter.cpp
