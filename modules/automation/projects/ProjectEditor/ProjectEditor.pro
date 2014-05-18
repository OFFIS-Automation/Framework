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

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins
TARGET = ProjectEditor

LIBS        += -L$${targetDir}/plugins -lqscintilla2

INCLUDEPATH += ../../../frontend/include
INCLUDEPATH += ../QScintilla/Qt4Qt5
INCLUDEPATH += ../../include


HEADERS += \
    src/ProjectEditor.h \
    src/HilecSingleton.h \
    src/FileTree/FileTree.h \
    src/Editor/EditArea.h \
    src/EditorToolBar.h \
    src/Editor/FileEditor.h \
    src/FileTree/FileModelFilterProxy.h

SOURCES += \
    src/ProjectEditor.cpp \
    src/HilecSingleton.cpp \
    src/FileTree/FileTree.cpp \
    src/Editor/EditArea.cpp \
    src/EditorToolBar.cpp \
    src/Editor/FileEditor.cpp \
    src/FileTree/FileModelFilterProxy.cpp

FORMS += \
    src/FileTree/FileTree.ui \
    src/Editor/EditArea.ui \
    src/EditorToolBar.ui

RESOURCES += \
    images/images.qrc







