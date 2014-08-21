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
# Project created by QtCreator 2011-03-01T15:29:08
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Framework
TEMPLATE = app
INCLUDEPATH += $${PWD}

INCLUDEPATH += ../../include
INCLUDEPATH += ../../projects/LogWidget
INCLUDEPATH += ../../../version

macx{
    ICON = images/icon.icns
}

win32-msvc*{
    RC_FILE = appIcon.rc
}

# Generate version number
MAJOR = 1
MINOR = 0
VERSION_HEADER = $${PWD}/../../../version/version.h
versiontarget.target = $$VERSION_HEADER

win32-msvc*{
    versiontarget.commands = $${PWD}/../../../version/version.exe $$MAJOR $$MINOR $$VERSION_HEADER
} else {
    versiontarget.commands = $${PWD}/../../../version/version $$MAJOR $$MINOR $$VERSION_HEADER
}
versiontarget.depends = FORCE
QMAKE_EXTRA_TARGETS += versiontarget
PRE_TARGETDEPS += $$VERSION_HEADER

include(../../../properties/pathes.pro)

DESTDIR = $${targetDir}

LIBS += -L$${targetDir} -lLogWidget

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/PluginLoader.cpp \
    src/DockWidgetTitle.cpp \
    src/PerspectiveControl.cpp \
    src/Application.cpp \
    src/Perspective.cpp \
    src/TranslationLoader.cpp \
    src/MasterWindow.cpp \
    src/StartScreen.cpp \
    src/newProjectWizard/CreateProjectDialog.cpp \
    src/newProjectWizard/CreateProjectPage1.cpp \
    src/newProjectWizard/CreateProjectPage2.cpp \
    src/AboutScreen.cpp

HEADERS  += src/MainWindow.h \ 
    ../../include/interfaces/GuiPluginInterface.h \
    ../../include/interfaces/ModelPluginInterface.h \
    src/PluginLoader.h \
    ../../include/interfaces/logging.h \
    src/DockWidgetTitle.h \
    src/PerspectiveControl.h \
    src/Application.h \
    src/Perspective.h \
    src/TranslationLoader.h \
    src/MasterWindow.h \
    src/StartScreen.h \
    src/newProjectWizard/CreateProjectDialog.h \
    src/newProjectWizard/CreateProjectPage1.h \
    src/newProjectWizard/CreateProjectPage2.h \
    src/AboutScreen.h

FORMS    += \
    src/DockWidgetTitle.ui \
    src/MasterWindow.ui \
    src/StartScreen.ui \
    src/newProjectWizard/CreateProjectDialog.ui \
    src/AboutScreen.ui

RESOURCES += \
    images/images.qrc

TRANSLATIONS = $${translationDir}/de/frontend_de.ts \
    $${translationDir}/stub/frontend_stub.ts
