
TEMPLATE = lib
DEFINES += FILTERBASE_LIBRARY

VPATH += $$PWD

# Timer
INCLUDEPATH += $$PWD/../../include

!contains(DEFINES, NO_SENSOR_SYSTEM) || !contains(DEFINES, NO_TIME_TRACER){
    INCLUDEPATH += $$PWD/../../../sensorSystem/include
    LIBS += -L$${targetDir}/plugins/ -lSensorSystem
}

!contains(DEFINES, NO_SENSOR_SYSTEM){
    SOURCES += src/OlvisSensorProvider.cpp
    HEADERS += src/OlvisSensorProvider.h
}


SOURCES += \
    src/UserFilterPrivate.cpp \
    src/UserFilter.cpp \
    src/OutputPortPrivate.cpp \
    src/OutputPort.cpp \
    src/InputPortPrivate.cpp \
    src/InputPort.cpp \
    src/BasePort.cpp \
    src/Filter.cpp \
    src/TypeInfoHelper.cpp \
    src/Tracer.cpp \
    src/Port.cpp \
    src/PortData.cpp \
    src/SrcFilter.cpp


HEADERS += \
    src/UserFilterPrivate.h \
    src/OutputPortPrivate.h \
    src/InputPortPrivate.h \
    src/BasePort.h \
    ../../include/filter/UserFilter.h \
    ../../include/filter/FilterBaseGlobal.h \
    ../../include/core/Filter.h \
    ../../include/core/ProcessorInfo.h \
    ../../include/core/PortInfo.h \
    ../../include/core/FilterTypeInfo.h \
    ../../include/core/PortListener.h \
    ../../include/filter/FilterTypeInfoHelper.h \
    ../../include/filter/InputPortMode.h \
    ../../include/core//Tracer.h \
    ../../include/filter/Port.h \
    src/PortData.h \
    ../../include/core/InputPort.h \
    ../../include/core/SrcFilter.h \
    ../../include/core/OutputPort.h

CONFIG+=dll

RESOURCES += \
    resource/resource.qrc
