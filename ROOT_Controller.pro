#-------------------------------------------------
#
# Project created by QtCreator 2011-12-06T14:42:29
#
#-------------------------------------------------

QT       += core \
    sql

QT       -= gui

TARGET = ROOT_Controller
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    DB_Operation.cpp \
    controllerdaemon.cpp \
    connectionprocess.cpp

HEADERS += \
    controllerdaemon.h \
    connectionprocess.h \
    DB_Operation.h

OTHER_FILES += \
    create_tables.sql
