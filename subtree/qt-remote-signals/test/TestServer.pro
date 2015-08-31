QT       += core network

TARGET = TestServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $${PWD}/../bin

SOURCES += server.cpp \
    TestDataServerBase.cpp \
    RemoteSignals.cpp \
    Handler.cpp

HEADERS += \
    TestDataServer.h \
    TestDataServerBase.h \
    RemoteSignals.h \
    Handler.h
