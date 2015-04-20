VPATH += $$PWD

DEFINES += DEFAULTPORTS_LIBRARY
INCLUDEPATH += $$PWD/../../include

CONFIG+=dll
TEMPLATE = lib

SOURCES += \ 
    src/BilateralFilter.cpp \
    src/BlobDetection.cpp \
    src/Blur.cpp \
    src/cannyfilter.cpp \
    src/ColorCorrection.cpp \
    src/CorrectPosition.cpp \
    src/Delay.cpp \
    src/Denoising.cpp \
    src/DirectoryIterator.cpp \
    src/FetchTemplate.cpp \
    src/FlipImage.cpp \
    src/GaussianBlur.cpp \
    src/Gray2Rgb.cpp \
    src/HistogramNode.cpp \
    src/ImageInput.cpp \
    src/ImageOutput.cpp \
    src/ImageSubstraction.cpp \
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
    src/Timer.cpp \
    src/VarianceFilter.cpp \
    src/VideoInput.cpp \
    src/VideoWriter.cpp \
    src/BrighnessContrast.cpp \
    src/BackgroundSubstraction.cpp \
    src/ColorExtraction.cpp \
    src/VignettingCorrection.cpp \
    src/MultipleTemplateMatching.cpp

HEADERS += \ 
    src/BilateralFilter.h \
    src/BlobDetection.h \
    src/Blur.h \
    src/cannyfilter.h \
    src/ColorCorrection.h \
    src/CorrectPosition.h \
    src/Delay.h \
    src/Denoising.h \
    src/DirectoryIterator.h \
    src/FetchTemplate.h \
    src/FlipImage.h \
    src/GaussianBlur.h \
    src/Gray2Rgb.h \
    src/HistogramNode.h \
    src/ImageInput.h \
    src/ImageSubstraction.h \
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
    src/Timer.h \
    src/VarianceFilter.h \
    src/VideoInput.h \
    src/VideoWriter.h \
    src/BrighnessContrast.h \
    src/BackgroundSubstraction.h \
    src/ColorExtraction.h \
    src/VignettingCorrection.h \
    src/MultipleTemplateMatching.h

RESOURCES += \
    images/images.qrc