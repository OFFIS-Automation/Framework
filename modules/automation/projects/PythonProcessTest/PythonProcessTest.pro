#-------------------------------------------------
#
# Project created by QtCreator 2013-07-26T13:46:38
#
#-------------------------------------------------

QT       += core

QT       += network

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins

INCLUDEPATH += ../../../frontend/include
INCLUDEPATH += ../../include

SIGNALDEFS = $${PWD}/../PythonProcess/signalProxy/PythonProcess.slots
createSignalProxy.target = $$PWD/src/signalProxy/PythonProcessClient.h


win32-msvc*{
    createSignalProxy.commands = $${PWD}/../../../SignalProxy/bin/SignalProxy.exe $$SIGNALDEFS $$PWD/src/signalProxy --client-only
} else {
    createSignalProxy.commands = $${PWD}/../../../SignalProxy/bin/SignalProxy $$SIGNALDEFS $$PWD/src/signalProxy --client-only
}
createSignalProxy.depends = FORCE
QMAKE_EXTRA_TARGETS += createSignalProxy
PRE_TARGETDEPS += $$PWD/src/signalProxy/PythonProcessClient.h


TARGET = PythonProcessTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    src/signalProxy/PythonProcessClient.cpp \
    src/TestClient.cpp \
    src/signalProxy/SignalProxy.cpp

HEADERS += \
    src/signalProxy/PythonProcessClient.h \
    src/TestClient.h \
    src/signalProxy/SignalProxy.h
