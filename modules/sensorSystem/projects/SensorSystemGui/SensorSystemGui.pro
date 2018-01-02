# OFFIS Automation Framework
# Copyright (C) 2013-2018 OFFIS e.V.
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

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins
TARGET = SensorSystemGui

QT += core gui widgets

INCLUDEPATH += ../../../frontend/include
INCLUDEPATH += ../../include
INCLUDEPATH += ../SensorTracer/src

LIBS += -L$${targetDir}/plugins -lSensorSystem -lSensorTracer

HEADERS += \
    src/SensorSystemGui.h \
    src/SensorWidget.h \
    src/AddSensorTraceDialog.h \
    src/SensorWidgetLine.h \
    src/ExportSensorTraceDialog.h

SOURCES += \
    src/SensorSystemGui.cpp \
    src/SensorWidget.cpp \
    src/AddSensorTraceDialog.cpp \
    src/SensorWidgetLine.cpp \
    src/ExportSensorTraceDialog.cpp

FORMS += \
    src/SensorWidget.ui \
    src/AddSensorTraceDialog.ui \
    src/SensorWidgetLine.ui \
    src/ExportSensorTraceDialog.ui

RESOURCES += \
    images/images.qrc
