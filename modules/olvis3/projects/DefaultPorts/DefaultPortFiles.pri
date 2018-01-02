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

QT += core gui widgets

TEMPLATE = lib

VPATH += $$PWD

INCLUDEPATH += $$PWD/../../include
INCLUDEPATH += $$PWD/../../../olvis3/include

#DEFINES += NO_SENSOR_SYSTEM

!contains(DEFINES, NO_SENSOR_SYSTEM){
    INCLUDEPATH += $$PWD/../../../sensorSystem/include
    INCLUDEPATH += $$PWD/../../../sensorSystem/projects/SensorTracer/src
    LIBS += -L$${targetDir}/plugins/ -lSensorSystem -lSensorTracer
}

CONFIG+=dll

HEADERS += \
    ../../include/gui/PortEditInterface.h \
    ../../include/gui/OlvisGuiPluginInterface.h \
    ../../include/gui/OverlayInterface.h \
    src/DefaultPorts.h \
    src/BoolEdit.h \
    src/AbstractPortEditWidget.h \
    src/AbstractPortEdit.h \
    src/DirectoryEdit.h \
    src/FileEdit.h \
    src/StringEdit.h \
    src/NumberEdit.h \
    src/DoubleEdit.h \
    src/ChoiceEdit.h \
    src/RectEdit.h \
    src/PointEdit.h \
    src/DateTimeEdit.h \
    src/overlays/RectOverlay.h \
    src/overlays/StringOverlay.h \
    src/overlays/ImagePortOverlay.h \
    src/overlays/Overlay.h \
    src/overlays/RectPortOverlay.h \
    src/overlays/DataOverlay.h \
    src/overlays/SimpleShapeOverlay.h \
    src/overlays/MainOverlay.h \
    src/overlays/HistogramOverlay.h \
    src/overlays/GraphOverlay.h \
    src/overlays/SensorSystemOverlay.h \
    src/overlays/ScaleBarOverlay.h \
    src/overlays/GraphOverlayOptions.h \
    src/overlays/PointPortOverlay.h \
    $$PWD/src/overlays/MeasureOverlay.h \
    $$PWD/src/ColorEdit.h


SOURCES += \
    src/DefaultPorts.cpp \
    src/BoolEdit.cpp \
    src/AbstractPortEditWidget.cpp \
    src/AbstractPortEdit.cpp \
    src/DirectoryEdit.cpp \
    src/FileEdit.cpp \
    src/StringEdit.cpp \
    src/NumberEdit.cpp \
    src/DoubleEdit.cpp \
    src/ChoiceEdit.cpp \
    src/RectEdit.cpp \
    src/PointEdit.cpp \
    src/DateTimeEdit.cpp \
    src/overlays/StringOverlay.cpp \
    src/overlays/ImagePortOverlay.cpp \
    src/overlays/Overlay.cpp \
    src/overlays/RectPortOverlay.cpp \
    src/overlays/DataOverlay.cpp \
    src/overlays/RectOverlay.cpp \
    src/overlays/SimpleShapeOverlay.cpp \
    src/overlays/MainOverlay.cpp \
    src/overlays/HistogramOverlay.cpp \
    src/overlays/GraphOverlay.cpp \
    src/overlays/SensorSystemOverlay.cpp \
    src/overlays/ScaleBarOverlay.cpp \
    src/overlays/GraphOverlayOptions.cpp \
    src/overlays/PointPortOverlay.cpp \
    $$PWD/src/overlays/MeasureOverlay.cpp \
    $$PWD/src/ColorEdit.cpp


FORMS += \
    src/AbstractPortEditWidget.ui \
    src/overlays/GraphOverlayOptions.ui \

RESOURCES += \
    images/images.qrc
