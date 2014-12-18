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

#-------------------------------------------------
#
# Project created by QtCreator 2011-03-01T15:35:22
#
#-------------------------------------------------

TEMPLATE = lib

QT += core gui widgets

win32-msvc*{
    contains(QT_CONFIG, opengles.) {
        DEFINES += OPENGL_ES
    } else {
        QT += opengl
        DEFINES += OPEN_GL
        message("Build with OpenGL")
    }
}

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins
TARGET = olvisExecGui
LIBS += -L$${targetDir}/plugins -lolvisFilter -lolvisPorts

include(../../../properties/opencv.pro)

INCLUDEPATH += $${PWD}
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../frontend/include

INCLUDEPATH += ../../../sensorSystem/include
INCLUDEPATH += ../../../sensorSystem/projects/SensorTracer/src
LIBS += -L$${targetDir}/plugins/ -lSensorSystem -lSensorTracer


SOURCES += \  
    src/OlvisExecGuiPlugin.cpp \
    src/MainToolBar.cpp \
    src/VideoDisplay/VideoWidget.cpp \
    src/VideoDisplay/VideoDisplayWidget.cpp \
    src/VideoDisplay/FilePortOverlay.cpp \
    src/VideoDisplay/RectOverlay.cpp \
    src/OlvisSingleton.cpp \
    src/VideoDisplay/StringOverlay.cpp \
    src/VideoDisplay/ImagePortOverlay.cpp \
    src/VideoDisplay/OverlayFactory.cpp \
    src/VideoDisplay/VideoDisplayScrollArea.cpp \
    src/VideoDisplay/Overlay.cpp \
    src/VideoDisplay/RectPortOverlay.cpp \
    src/VideoDisplay/VideoControlToolbar.cpp \
    src/VideoDisplay/VideoRecorder.cpp \
    src/VideoDisplay/DataOverlay.cpp \
    src/VideoDisplay/SimpleShapeOverlay.cpp \
    src/dialogs/ExportTraceDialog.cpp \
    src/VideoDisplay/PointPortOverlay.cpp \
    src/VideoDisplay/MainOverlay.cpp \
    src/VideoDisplay/HistogramOverlay.cpp \
    src/VideoDisplay/GraphOverlay.cpp \
    src/dialogs/GraphOverlayOptions.cpp \
    src/VideoDisplay/SensorSystemOverlay.cpp \
    src/VideoDisplay/ScaleBarOverlay.cpp \
    src/dialogs/OlvisOptionsGui.cpp


HEADERS +=  \
    src/OlvisExecGuiPlugin.h \
    src/MainToolBar.h \
    src/VideoDisplay/VideoWidget.h \
    src/VideoDisplay/VideoDisplayWidget.h \
    src/VideoDisplay/FilePortOverlay.h \
    src/VideoDisplay/RectOverlay.h \
    src/OlvisSingleton.h \
    src/VideoDisplay/StringOverlay.h \
    src/VideoDisplay/ImagePortOverlay.h \
    src/VideoDisplay/OverlayFactory.h \
    src/VideoDisplay/VideoDisplayScrollArea.h \
    src/VideoDisplay/Overlay.h \
    src/VideoDisplay/RectPortOverlay.h \
    src/VideoDisplay/VideoControlToolbar.h \
    src/VideoDisplay/VideoRecorder.h \
    src/VideoDisplay/DataOverlay.h \
    src/VideoDisplay/SimpleShapeOverlay.h \
    src/dialogs/ExportTraceDialog.h \
    src/VideoDisplay/PointPortOverlay.h \
    src/VideoDisplay/MainOverlay.h \
    src/VideoDisplay/HistogramOverlay.h \
    src/VideoDisplay/GraphOverlay.h \
    src/dialogs/GraphOverlayOptions.h \
    src/VideoDisplay/SensorSystemOverlay.h \
    src/VideoDisplay/ScaleBarOverlay.h \
    src/dialogs/OlvisOptionsGui.h
       

FORMS += \
    src/MainToolBar.ui \
    src/VideoDisplay/VideoWidget.ui \
    src/VideoDisplay/VideoControlToolbar.ui \
    src/dialogs/ExportTraceDialog.ui \
    src/dialogs/GraphOverlayOptions.ui \
    src/dialogs/OlvisOptionsGui.ui

RESOURCES += \
    images/images.qrc
