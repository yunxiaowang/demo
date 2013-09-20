#-------------------------------------------------
#
# Project created by QtCreator 2012-11-09T10:33:30
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlashContainerLinux
TEMPLATE = app

INCLUDEPATH += ./npapiinclude

DEFINES += HAVE_NETINET_IN_H

SOURCES += main.cpp\
    FlashPlugin.cpp \
    Container.cpp \
    npn.cpp \
    FlashContentLoader.cpp \
    faststrdup.cpp \
    Identifier.cpp \
    LocationNPClass.cpp \
    WindowNPClass.cpp

HEADERS  += \
    FlashPlugin.h \
    npapiinclude/npfunctions.h \
    npapiinclude/npapi.h \
    npn.h \
    npapiinclude/nptypes.h \
    npapiinclude/npruntime.h \
    Container.h \
    FlashContentLoader.h \
    FRException.h \
    faststrdup.h \
    Identifier.h \
    LocationNPClass.h \
    WindowNPClass.h


OTHER_FILES += \
    libflashplayer.so
