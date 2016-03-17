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
# Project created by QtCreator 2011-03-01T15:29:08
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Framework
TEMPLATE = app
INCLUDEPATH += $${PWD}

INCLUDEPATH += ../../include
INCLUDEPATH += ../Notifications/include
INCLUDEPATH += ../../projects/LogWidget

macx{
    ICON = images/icon.icns
}

win32-msvc*{
    RC_FILE = appIcon.rc
}

DEFINES += MAJOR_VERSION=0
DEFINES += MINOR_VERSION=3
DEFINES += PATCH_VERSION=3

include(../../../properties/pathes.pro)

DESTDIR = $${targetDir}

LIBS += -L$${targetDir} -L$${targetDir}/plugins -lLogWidget -lNotifications

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
    src/AboutScreen.h \
    src/version.h

FORMS   += \
    src/DockWidgetTitle.ui \
    src/MasterWindow.ui \
    src/StartScreen.ui \
    src/newProjectWizard/CreateProjectDialog.ui \
    src/AboutScreen.ui

RESOURCES += \
    images/images.qrc

TRANSLATIONS = $${translationDir}/de/frontend_de.ts \
    $${translationDir}/stub/frontend_stub.ts

dlls.path  =  $${DESTDIR}
dllA.path   += $${DESTDIR}/platforms
dllB.path   += $${DESTDIR}/imageformats/
dllC.path   += $${DESTDIR}/iconengines/

dlls.files += $$[QT_INSTALL_BINS]/d3dcompiler_47.dll
dlls.files += $$[QT_INSTALL_BINS]/opengl32sw.dll

CONFIG(debug, debug|release) {
    dlls.files += $$[QT_INSTALL_BINS]/icudt5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuin5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuuc5*.dll

    dlls.files += $$[QT_INSTALL_BINS]/libGLESv2d.dll
    dlls.files += $$[QT_INSTALL_BINS]/libEGLd.dll

    dlls.files += $$[QT_INSTALL_BINS]/Qt5Cored.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Guid.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Networkd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Multimediad.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5OpenGld.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5PrintSupportd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5SerialPortd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Svgd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgetsd.dll

    dllA.files  += $$[QT_INSTALL_PLUGINS]/platforms/qwindowsd.dll

    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qddsd.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qgifd.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qicnsd.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qicod.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/jpegd.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qtgad.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qtiffd.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qwbmpd.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qwebpd.dll

    dllC.files  += $$[QT_INSTALL_PLUGINS]/iconengines/qsvgicond.dll
} else {
    dlls.files += $$[QT_INSTALL_BINS]/icudt5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuin5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuuc5*.dll

    dlls.files += $$[QT_INSTALL_BINS]/libGLESv2.dll
    dlls.files += $$[QT_INSTALL_BINS]/libEGL.dll

    dlls.files += $$[QT_INSTALL_BINS]/Qt5Core.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Gui.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Network.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Multimedia.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5OpenGl.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5PrintSupport.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5SerialPort.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Svg.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgets.dll

    dllA.files  += $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll

    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qdds.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qgif.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qicns.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qico.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/jpeg.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qtga.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qtiff.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qwbmp.dll
    dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qwebp.dll

    dllC.files  += $$[QT_INSTALL_PLUGINS]/iconengines/qsvgicon.dll
}
INSTALLS   += dlls dllA dllB dllC
