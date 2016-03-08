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

#QT       -= gui

TEMPLATE = lib

VPATH += $$PWD

INCLUDEPATH += $$PWD/../../include
INCLUDEPATH += $$PWD/../olvisCore/src
INCLUDEPATH += $$PWD/../../../frontend/include

opencvdlls.path   = $${DESTDIR}
opencvdlls.files += $${CVBINDIR}/*.dll
INSTALLS         += opencvdlls

# Without this line, the VideoInput filter will not run
# inside a deployed app version. More information here:
# http://answers.opencv.org/question/62156/python-videocapture-isopened-always-return-false/
opencv_video.path    = $${DESTDIR}/../
opencv_video.files  += $${CVBINDIR}/opencv_ffmpeg*.dll
INSTALLS            += opencv_video

HEADERS += \
    src/OlvisModelPlugin.h \
    ../../../frontend/include/interfaces/ModelPluginInterface.h

SOURCES += \
    src/OlvisModelPlugin.cpp
