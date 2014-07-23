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


win32-msvc*{
    CVINCLUDEDIR = $$(AmirDevDir)/opencv2/include
    CVLIBDIR = $$(AmirDevDir)/opencv2/lib
    CVBINDIR = $$(AmirDevDir)/opencv2/bin
    INCLUDEPATH += $$CVINCLUDEDIR
    DEFINES += _CRT_SECURE_NO_WARNINGS # supress win32 stdlib warnings
    include($$(AmirDevDir)/opencv2/opencv_version.pro)
    CONFIG(debug, debug|release) {
        LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION}d -lopencv_imgproc$${CVVERSION}d -lopencv_highgui$${CVVERSION}d
    } else {
        LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION} -lopencv_imgproc$${CVVERSION} -lopencv_highgui$${CVVERSION}
    }

}

macx{
    message("Including OpenCV 2.4.2 Mac OS X")
    CVINCLUDEDIR = /usr/local/include/opencv2
    CVLIBDIR = /usr/local/lib
    CVVERSION = .2.4.2
    debug {
        LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION} -lopencv_imgproc$${CVVERSION} -lopencv_highgui$${CVVERSION}
    } else {
        LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION} -lopencv_imgproc$${CVVERSION} -lopencv_highgui$${CVVERSION}
    }
}

unix:!macx{
    message("Including OpenCV 2.4.2 UNIX")
    unix:!macx:CONFIG += link_pkgconfig
    unix:!macx:PKGCONFIG += opencv
    LIBS += `pkg-config opencv --libs --cflags`
}
