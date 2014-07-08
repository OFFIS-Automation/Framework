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

#-------------------------------------------------
#
# Project created by QtCreator 2010-12-17T14:40:48
#
#-------------------------------------------------

#QT       -= gui


TEMPLATE = lib

DEFINES += DEFAULTPORTS_LIBRARY
INCLUDEPATH += ../../include

CONFIG+=dll

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins/olvisPlugins
TARGET = SimpleNodes
LIBS += -L$${PWD}/lib -L$${targetDir}/plugins -lolvisFilter -lolvisPorts  -lolvisPlugin

INCLUDEPATH += ../../../sensorSystem/include
LIBS += -L$${targetDir}/plugins/ -lSensorSystem

include(../../../properties/opencv.pro)

SOURCES += \ 
    src/BlobDetection.cpp \
    src/Blur.cpp \
    src/cannyfilter.cpp \
    src/ColorCorrection.cpp \
    src/CorrectPosition.cpp \
    src/Delay.cpp \
    src/DirectoryIterator.cpp \
    src/FetchTemplate.cpp \
    src/FlipImage.cpp \
    src/GaussianBlur.cpp \
    src/Gray2Rgb.cpp \
    src/HistogramNode.cpp \
    src/ImageInput.cpp \
    src/ImageOutput.cpp \
    src/Invert.cpp \
    src/main.cpp \
    src/MathFilter.cpp \
    src/MedianBlur.cpp \
    src/MinimumFps.cpp \
    src/Morphology.cpp \
    src/NormalizeHistogramm.cpp \
    src/PathCreator.cpp \
    src/PointInput.cpp \
    src/RectInput.cpp \
    src/ResizeImage.cpp \
    src/Rgb2Gray.cpp \
    src/RotateImage.cpp \
    src/ScaleImage.cpp \
    src/SetRoiFilter.cpp \
    src/SimpleCameraInput.cpp \
    src/SimpleNodes.cpp \
    src/SobelFilter.cpp \
    src/TemplateMatching.cpp \
    src/Threshold.cpp \
    src/VarianceFilter.cpp \
    src/VideoInput.cpp \
    src/VideoWriter.cpp \
    src/BrighnessContrast.cpp

HEADERS += \ 
    src/BlobDetection.h \
    src/Blur.h \
    src/cannyfilter.h \
    src/ColorCorrection.h \
    src/CorrectPosition.h \
    src/Delay.h \
    src/DirectoryIterator.h \
    src/FetchTemplate.h \
    src/FlipImage.h \
    src/GaussianBlur.h \
    src/Gray2Rgb.h \
    src/HistogramNode.h \
    src/ImageInput.h \
    src/ImageInput.h \
    src/ImageOutput.h \
    src/Invert.h \
    src/KernelFilter.h \
    src/MathFilter.h \
    src/MedianBlur.h \
    src/MinimumFps.h \
    src/Morphology.h \
    src/NormalizeHistogramm.h \
    src/PathCreator.h \
    src/PointInput.h \
    src/RectInput.h \
    src/ResizeImage.h \
    src/Rgb2Gray.h \
    src/RotateImage.h \
    src/ScaleImage.h \
    src/SetRoiFilter.h \
    src/SimpleCameraInput.h \
    src/SimpleNodes.h \
    src/SobelFilter.h \
    src/TemplateMatching.h \
    src/Threshold.h \
    src/VarianceFilter.h \
    src/VideoInput.h \
    src/VideoWriter.h \
    src/BrighnessContrast.h

RESOURCES += \
    images/images.qrc
