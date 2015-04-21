#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T14:19:33
#
#-------------------------------------------------
VPATH += $$PWD

QT       += gui widgets

TARGET = Notifications
TEMPLATE = lib

DEFINES += NOTIFICATIONS_LIBRARY

SOURCES += \
    src/NotificationMainWidget.cpp \
    src/Notifications.cpp \
    src/NotificationWidget.cpp

HEADERS += \
    src/NotificationMainWidget.h \
    src/NotificationWidget.h \
    include/Notifications.h

FORMS += \
    src/NotificationMainWidget.ui \
    src/NotificationWidget.ui

RESOURCES += \
    $$PWD/images/images.qrc

DISTFILES += \
    $$PWD/images/close.png
