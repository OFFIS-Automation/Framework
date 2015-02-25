include(../../../properties/pathes.pro)
include(../../../properties/opencv.pro)

TEMPLATE = lib
CONFIG += dll
TARGET = MeasureNodes
DESTDIR = $${targetDir}/plugins/olvisPlugins

INCLUDEPATH += ../../include
LIBS += -L$${PWD}/lib -L$${targetDir}/plugins -lolvisFilter -lolvisPorts  -lolvisPlugin



HEADERS += \
    src/MeasureNodes.h \
	src/Size.h

SOURCES += \
    src/MeasureNodes.cpp \
	src/Size.cpp
    
HEADERS += src/PhysicalImageSize.h
SOURCES += src/PhysicalImageSize.cpp
