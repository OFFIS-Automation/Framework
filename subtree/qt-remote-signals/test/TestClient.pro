#-------------------------------------------------
#
# Project created by QtCreator 2013-10-16T12:15:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestClient
TEMPLATE = app


SOURCES += client.cpp\
        ClientGui.cpp \
        TestDataClientBase.cpp \
    RemoteSignals.cpp

HEADERS  += ClientGui.h \
    TestDataClient.h \
    TestDataClientBase.h \
    RemoteSignals.h

FORMS    += ClientGui.ui
