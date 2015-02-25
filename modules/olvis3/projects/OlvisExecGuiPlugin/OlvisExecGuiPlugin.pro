# OFFIS Automation Framework
# Copyright (C) 2013-2015 OFFIS e.V.
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
    src/OlvisSingleton.cpp \
    src/VideoDisplay/VideoDisplayScrollArea.cpp \
    src/VideoDisplay/VideoControlToolbar.cpp \
    src/VideoDisplay/VideoRecorder.cpp \
    src/dialogs/ExportTraceDialog.cpp \
    src/dialogs/OlvisOptionsGui.cpp \
    ../OlvisGuiPlugin/src/PluginContainer.cpp


HEADERS +=  \
    src/OlvisExecGuiPlugin.h \
    src/MainToolBar.h \
    src/VideoDisplay/VideoWidget.h \
    src/VideoDisplay/VideoDisplayWidget.h \
    src/OlvisSingleton.h \
    src/VideoDisplay/VideoDisplayScrollArea.h \
    src/VideoDisplay/VideoControlToolbar.h \
    src/VideoDisplay/VideoRecorder.h \
    src/dialogs/ExportTraceDialog.h \
    src/dialogs/OlvisOptionsGui.h \
    ../../include/gui/OverlayInterface.h \
    ../OlvisGuiPlugin/src/PluginContainer.h
       

FORMS += \
    src/MainToolBar.ui \
    src/VideoDisplay/VideoWidget.ui \
    src/VideoDisplay/VideoControlToolbar.ui \
    src/dialogs/ExportTraceDialog.ui \
    src/dialogs/OlvisOptionsGui.ui

RESOURCES += \
    images/images.qrc
