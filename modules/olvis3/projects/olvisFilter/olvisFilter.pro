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

#-------------------------------------------------
#
# Project created by QtCreator 2010-12-17T13:59:31
#
#-------------------------------------------------



TEMPLATE = lib
DEFINES += FILTERBASE_LIBRARY
include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins
TARGET = olvisFilter

# Timer
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../sensorSystem/include
LIBS += -L$${targetDir}/plugins/ -lSensorSystem

SOURCES += \ 
    src/UserFilterPrivate.cpp \
    src/UserFilter.cpp \
    src/OutputPortPrivate.cpp \
    src/OutputPort.cpp \
    src/InputPortPrivate.cpp \
    src/InputPort.cpp \
    src/BasePort.cpp \
    src/Filter.cpp \
    src/TypeInfoHelper.cpp \
    src/Tracer.cpp \
    src/Port.cpp \
    src/PortData.cpp \
    src/OlvisSensorProvider.cpp \
    src/SrcFilter.cpp


HEADERS += \ 
    src/UserFilterPrivate.h \
    src/OutputPortPrivate.h \
    src/InputPortPrivate.h \
    src/BasePort.h \
    ../../include/filter/UserFilter.h \
    ../../include/filter/FilterBaseGlobal.h \
    ../../include/core/Filter.h \
    ../../include/core/ProcessorInfo.h \
    ../../include/core/PortInfo.h \
    ../../include/core/FilterTypeInfo.h \
    ../../include/core/PortListener.h \
    ../../include/filter/FilterTypeInfoHelper.h \
    ../../include/filter/InputPortMode.h \
    ../../include/core//Tracer.h \
    ../../include/filter/Port.h \
    src/PortData.h \
    ../../include/core/InputPort.h \
    src/OlvisSensorProvider.h \
    ../../include/core/SrcFilter.h \
    ../../include/core/OutputPort.h
	

CONFIG+=dll

RESOURCES += \
    resource/resource.qrc
