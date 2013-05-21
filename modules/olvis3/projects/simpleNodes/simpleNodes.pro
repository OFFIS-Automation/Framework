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


include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins/olvisPlugins
TARGET = SimpleNodes
LIBS += -L$${PWD}/lib -L$${targetDir}/plugins -lolvisFilter -lolvisPorts  -lolvisPlugin

INCLUDEPATH += ../../../sensorSystem/include
LIBS += -L$${targetDir}/plugins/ -lSensorSystem

include(../../../properties/opencv.pro)

SOURCES += \ 
    src/Threshold.cpp \
    src/DirectoryIterator.cpp \
    src/main.cpp \
    src/ImageOutput.cpp \
    src/PathCreator.cpp \
    src/Delay.cpp \
    src/MinimumFps.cpp \
    src/VideoInput.cpp \
    src/SetRoiFilter.cpp \
    src/SimpleCameraInput.cpp \
    src/MathFilter.cpp \
    src/MedianBlur.cpp \
    src/GaussianBlur.cpp \
    src/Morphology.cpp \
    src/cannyfilter.cpp \
    src/Rgb2Gray.cpp \
    src/Invert.cpp \
    src/ScaleImage.cpp \
    src/TemplateMatching.cpp \
    src/FlipImage.cpp \
    src/PointInput.cpp \
    src/RectInput.cpp \
    src/Gray2Rgb.cpp \
    src/ImageInput.cpp \
    src/Blur.cpp \
    src/HistogramNode.cpp \
    src/RotateImage.cpp \
    src/NormalizeHistogramm.cpp \
    src/SobelFilter.cpp \
    src/ResizeImage.cpp \
    src/SimpleNodes.cpp

HEADERS += \ 
    src/Threshold.h \
    src/ImageInput.h \
    src/DirectoryIterator.h \
    src/ImageOutput.h \
    src/PathCreator.h \
    src/Delay.h \
    src/MinimumFps.h \
    src/VideoInput.h \
    src/SetRoiFilter.h \
    src/SimpleCameraInput.h \
    src/MathFilter.h \
    src/KernelFilter.h \
    src/MedianBlur.h \
    src/GaussianBlur.h \
    src/Morphology.h \
    src/cannyfilter.h \
    src/Rgb2Gray.h \
    src/Invert.h \
    src/ScaleImage.h \
    src/TemplateMatching.h \
    src/FlipImage.h \
    src/PointInput.h \
    src/RectInput.h \
    src/Gray2Rgb.h \
    src/ImageInput.h \
    src/Blur.h \
    src/HistogramNode.h \
    src/RotateImage.h \
    src/NormalizeHistogramm.h \
    src/SobelFilter.h \
    src/ResizeImage.h \
    src/SimpleNodes.h

CONFIG+=dll

RESOURCES += \
    images/images.qrc
HEADERS += src/ColorCorrection.h
SOURCES += src/ColorCorrection.cpp
HEADERS += src/VarianceFilter.h
SOURCES += src/VarianceFilter.cpp
HEADERS += src/VideoWriter.h
SOURCES += src/VideoWriter.cpp
HEADERS += src/FetchTemplate.h
SOURCES += src/FetchTemplate.cpp
HEADERS += src/CorrectPosition.h
SOURCES += src/CorrectPosition.cpp
HEADERS += src/BlobDetection.h
SOURCES += src/BlobDetection.cpp
