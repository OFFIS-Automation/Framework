VPATH += $$PWD

PYLON_DEV_DIR = $$(PYLON_DEV_DIR)
isEmpty(PYLON_DEV_DIR) {
    error("Error: PYLON_DEV_DIR  not found. Please install Pylon SDK")
} else {
    message("Pylon SDK found at $${PYLON_DEV_DIR}");
}

TEMPLATE = lib
CONFIG += dll

INCLUDEPATH += $$(PYLON_DEV_DIR)/include
LIBS += -L$$(PYLON_DEV_DIR)/lib/Win32

message($$LIBS)

HEADERS += \
    src/PylonPlugin.h \
    src/PylonCamera.h \
    src/params/IntegerParameter.h \
    src/params/ParamInterface.h \
    src/params/BoolParameter.h \
    src/params/FloatParameter.h \
    src/params/EnumerationParameter.h \
    $$PWD/src/params/ParameterTemplate.h

SOURCES += \
    src/PylonPlugin.cpp \
    src/PylonCamera.cpp

DISTFILES += \
    README.txt
