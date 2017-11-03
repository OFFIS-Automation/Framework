# OFFIS Automation Framework
# Copyright (C) 2013-2017 OFFIS e.V.
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


win32-msvc*{
    CVINCLUDEDIR = $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/opencv3/include

    contains(QT_ARCH, i386):{
        CVLIBDIR = $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/opencv3/x86/vc15/lib
        CVBINDIR = $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/opencv3/x86/vc15/bin
    } else {
        CVLIBDIR = $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/opencv3/x64/vc15/lib
        CVBINDIR = $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/opencv3/x64/vc15/bin
    }

    INCLUDEPATH += $$CVINCLUDEDIR
    DEFINES += _CRT_SECURE_NO_WARNINGS # supress win32 stdlib warnings
    include($$(OFFIS_DEVELOPMENT_ENVIRONMENT)/opencv3/opencv_version.pro)
    CONFIG(debug, debug|release) {
        LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION}d -lopencv_imgcodecs$${CVVERSION}d -lopencv_imgproc$${CVVERSION}d -lopencv_highgui$${CVVERSION}d -lopencv_video$${CVVERSION}d -lopencv_videoio$${CVVERSION}d -lopencv_photo$${CVVERSION}d
    } else {
        LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION} -lopencv_imgcodecs$${CVVERSION} -lopencv_imgproc$${CVVERSION} -lopencv_highgui$${CVVERSION} -lopencv_video$${CVVERSION} -lopencv_videoio$${CVVERSION} -lopencv_photo$${CVVERSION}
    }
}
