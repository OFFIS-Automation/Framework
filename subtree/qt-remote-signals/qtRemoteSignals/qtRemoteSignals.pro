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

QT       += core

TARGET = qtRemoteSignals
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $${PWD}/../bin



SOURCES += src/main.cpp \
    ../bin/RemoteSignals.cpp

HEADERS += \
    ../bin/RemoteSignals.h

dlls.path  =  $${DESTDIR}
dllA.path   += $${DESTDIR}/platforms
dllB.path   += $${DESTDIR}/plugins/imageformats/

CONFIG(debug, debug|release) {
    dlls.files += $$[QT_INSTALL_BINS]/icudt5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuin5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuuc5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Cored.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Networkd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Guid.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgetsd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5OpenGld.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5PrintSupportd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5SerialPortd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgetsd.dll
} else {
    dlls.files += $$[QT_INSTALL_BINS]/icudt5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuin5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuuc5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Core.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Network.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Gui.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgets.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5OpenGl.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5PrintSupport.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5SerialPort.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgets.dll
}
dllA.files  += $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll
dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qico.dll
dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qwbmp.dll
INSTALLS   += dlls dllA dllB
