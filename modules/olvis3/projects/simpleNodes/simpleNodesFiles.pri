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

VPATH += $$PWD

QT  += multimedia multimediawidgets

DEFINES += DEFAULTPORTS_LIBRARY
INCLUDEPATH += $$PWD/../../include

CONFIG+=dll
TEMPLATE = lib

SOURCES += \
	src/BilateralFilter.cpp \
	src/BlobDetection.cpp \
	src/Blur.cpp \
	src/Canny.cpp \
	src/ColorCorrection.cpp \
	src/CorrectPosition.cpp \
        src/Clahe.cpp \
	src/Delay.cpp \
	src/DirectoryIterator.cpp \
	src/FetchTemplate.cpp \
	src/FlipImage.cpp \
        src/Frames.cpp \
	src/GaussianBlur.cpp \
	src/Gray2Rgb.cpp \
	src/HistogramNode.cpp \
	src/ImageInput.cpp \
	src/ImageOutput.cpp \
	src/ImageSubstraction.cpp \
        src/Inpainting.cpp \
	src/Invert.cpp \
	src/main.cpp \
	src/MedianBlur.cpp \
	src/MinimumFps.cpp \
	src/Morphology.cpp \
	src/NormalizeHistogramm.cpp \
	src/Overlay.cpp \
	src/PathCreator.cpp \
	src/PointInput.cpp \
        src/PoseInput.cpp \
	src/RectInput.cpp \
	src/ResizeImage.cpp \
	src/Rgb2Gray.cpp \
	src/Rgb2Graphen.cpp \
	src/RotateImage.cpp \
	src/ScaleImage.cpp \
        src/ScaleImage2.cpp \
        src/ScaleImage3.cpp \
	src/SetRoiFilter.cpp \
        src/Sharp.cpp \
	src/SimpleNodes.cpp \
	src/SobelFilter.cpp \
	src/TemplateMatching.cpp \
	src/Threshold.cpp \
        src/Timer.cpp \
	src/VarianceFilter.cpp \
	src/VideoInput.cpp \
	src/VideoWriter.cpp \
	src/BrighnessContrast.cpp \
	src/BackgroundSubstraction.cpp \
	src/ColorExtraction.cpp \
	src/VignetteCreator.cpp \
	src/VignettingCorrection.cpp \
	src/VignettingCorrectionRGB.cpp \
	src/WhiteBalance.cpp \
	src/MultipleTemplateMatching.cpp \
	src/ColorPicker.cpp \
        src/SimpleCameraCV.cpp \
        src/SimpleCameraQt.cpp \
        src/CameraFrameGrabber.cpp \
        src/VideoStreamer.cpp \
        src/VideoReceiver.cpp \
        src/GStreamerInitializer.cpp \
        src/OriginSink.cpp \

HEADERS += \
	src/BilateralFilter.h \
	src/BlobDetection.h \
	src/Blur.h \
	src/Canny.h \
	src/ColorCorrection.h \
	src/CorrectPosition.h \
        src/Clahe.h \
	src/Delay.h \
	src/DirectoryIterator.h \
	src/FetchTemplate.h \
	src/FlipImage.h \
        src/Frames.h \
	src/GaussianBlur.h \
	src/Gray2Rgb.h \
	src/Rgb2Graphen.h \
	src/HistogramNode.h \
	src/ImageInput.h \
	src/ImageSubstraction.h \
        src/Inpainting.h \
	src/ImageOutput.h \
	src/Invert.h \
	src/KernelFilter.h \
	src/MedianBlur.h \
	src/MinimumFps.h \
	src/Morphology.h \
	src/NormalizeHistogramm.h \
	src/Overlay.h \
	src/PathCreator.h \
	src/PointInput.h \
        src/PoseInput.h \
	src/RectInput.h \
	src/ResizeImage.h \
	src/Rgb2Gray.h \
	src/RotateImage.h \
	src/ScaleImage.h \
        src/ScaleImage2.h \
        src/ScaleImage3.h \
	src/SetRoiFilter.h \
        src/Sharp.h \
	src/SimpleNodes.h \
	src/SobelFilter.h \
	src/TemplateMatching.h \
	src/Threshold.h \
	src/Timer.h \
	src/VarianceFilter.h \
	src/VideoInput.h \
	src/VideoWriter.h \
	src/BrighnessContrast.h \
	src/BackgroundSubstraction.h \
	src/ColorExtraction.h \
	src/VignetteCreator.h \
	src/VignettingCorrection.h \
	src/VignettingCorrectionRGB.h \
	src/WhiteBalance.h \
	src/MultipleTemplateMatching.h \
	src/ColorPicker.h \
        src/SimpleCameraCV.h \
        src/SimpleCameraQt.h \
        src/CameraFrameGrabber.h \
        src/VideoStreamer.h \
        src/VideoReceiver.h \
        src/GStreamerInitializer.h \
        src/OriginSink.h \

RESOURCES += \
	images/images.qrc
