TEMPLATE = lib

VPATH += $$PWD

DEFINES += OVCORE_LIBRARY
INCLUDEPATH += $${PWD}/../../include
INCLUDEPATH += $${PWD}/../../../frontend/projects/Notifications/include
LIBS += -L$${targetDir}/plugins -L$${targetDir} -lolvisFilter -lolvisPorts -lNotifications

CONFIG+=dll

HEADERS += \
    src/OlvisCoreInterface.h \
    ../../include/core/ProcessorInfo.h \
    ../../include/core/PortInfo.h \
    ../../include/core/OlvisInterface.h \
    ../../include/core/FilterTypeInfo.h \
    ../../include/core/FilterInfo.h \
    ../../include/filter/UserFilter.h \
    src/CoreGlobal.h \
    ../../include/filter/FilterPluginInterface.h \
    src/Processor.h \
    src/ProcessorOutputPort.h \
    ../../include/core/PortId.h \
    ../../include/core/FilterConnection.h \
    ../../include/core/PortListener.h \
    src/ConfigWriter.h \
    src/ConfigReader.h \
    src/DataJoin.h \
    src/DataBuffer.h \
    src/ProcessorTriggerInterface.h \
    src/ProcessorInputPort.h \
    src/MakroFilter.h \
    src/FilterGroup.h

SOURCES += \
    src/OlvisCoreInterface.cpp \
    src/Processor.cpp \
    src/ConfigWriter.cpp \
    src/ConfigReader.cpp \
    src/DataJoin.cpp \
    src/DataBuffer.cpp \
    src/ProcessorOutputPort.cpp \
    src/ProcessorInputPort.cpp \
    src/MakroFilter.cpp \
    src/FilterGroup.cpp
