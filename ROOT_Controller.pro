#-------------------------------------------------
#
# Project created by QtCreator 2011-12-06T14:42:29
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ROOT_Controller
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    DB_Operation.cpp \
    controllerdaemon.cpp \
    connectionprocess.cpp \
    confighelper.cpp

HEADERS += \
    controllerdaemon.h \
    connectionprocess.h \
    confighelper.h
