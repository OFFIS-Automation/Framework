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
# Project created by QtCreator 2011-08-23T16:10:46
#
#-------------------------------------------------

TARGET = LogWidget
TEMPLATE = lib
CONFIG += staticlib
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$quote(../../include)

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}

HEADERS += \
    LogWindow.h \
    ExtendedTextEdit.h \
    LogSyntaxHighlighter.h

SOURCES += \
    LogWindow.cpp \
    ExtendedTextEdit.cpp \
    LogSyntaxHighlighter.cpp

FORMS += \
    LogWindow.ui

TRANSLATIONS = $${translationDir}/de/logWidget_de.ts \
    $${translationDir}/stub/logWidget_stub.ts





