VPATH += $$PWD

PYLON_ROOT = $$(PYLON_ROOT)
isEmpty(PYLON_ROOT) {
    error("Error: PYLON_ROOT not found. Please install Pylon SDK")
} else {
    message("Pylon found: $${PYLON_ROOT}");
}

GENICAM_ROOT = $$(PYLON_GENICAM_ROOT)
isEmpty(GENICAM_ROOT) {
    error("Error: PYLON_GENICAM_ROOT not found. Please install Pylon SDK")
} else {
    message("Pylon genicam found: $${GENICAM_ROOT}");
}

TEMPLATE = lib
CONFIG += dll

INCLUDEPATH += $$(PYLON_ROOT)/include
LIBS += -L$$(PYLON_ROOT)/lib/Win32
INCLUDEPATH += $$(PYLON_GENICAM_ROOT)/library/cpp/include
LIBS+= -L$$(PYLON_GENICAM_ROOT)/library/cpp/lib/win32_i86

message($$LIBS)

HEADERS += \
    src/PylonPlugin.h \
        src/PylonCamera.h \
    src/params/IntegerParameter.h \
    src/params/ParamInterface.h \
    src/params/BoolParameter.h \
    src/params/FloatParameter.h \
    src/params/EnumerationParameter.h

SOURCES += \
    src/PylonPlugin.cpp \
        src/PylonCamera.cpp \
    src/params/IntegerParameter.cpp \
    src/params/BoolParameter.cpp \
    src/params/FloatParameter.cpp \
    src/params/EnumerationParameter.cpp


DISTFILES += \
    README.txt
