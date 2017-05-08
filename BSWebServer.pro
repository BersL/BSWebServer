#-------------------------------------------------
#
# Project created by QtCreator 2017-04-19T15:24:11
#
#-------------------------------------------------

QT       += core gui
QT       += network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BSWebServer
TEMPLATE = app

LIBS += /usr/local/lib/libmagic.dylib

INCLUDEPATH += /usr/local/include/

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0


SOURCES += main.cc\
        mainwindow.cc \
    WebServerCore.cc \
    Response.cc \
    DataResponse.cc \
    ResponseManager.cc \
    ErrorResponse.cc \
    ConfigDialog.cc \
    ConnectionLogger.cc \
    StateIndicator.cc \
    ConnectionManager.cc \
    Request.cc \
    CommonUtils.cc \
    RequestManager.cc \
    FileResponse.cc \
    IncomingConnection.cc

HEADERS  += mainwindow.h \
    WebServerCore.h \
    Response.h \
    StatusCode.h \
    DataResponse.h \
    ResponseManager.h \
    ErrorResponse.h \
    ConfigDialog.h \
    ConnectionLogger.h \
    StateIndicator.h \
    ConnectionManager.h \
    Request.h \
    CommonUtils.h \
    RequestManager.h \
    FileResponse.h \
    IncomingConnection.h

FORMS    += mainwindow.ui \
    ConfigDialog.ui \
    StateIndicator.ui

DISTFILES += \
    index.html

RESOURCES +=
