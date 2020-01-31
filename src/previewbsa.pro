#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T19:50:56
#
#-------------------------------------------------

TARGET = previewBsa
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets gui

DEFINES += PREVIEWBSA_LIBRARY

SOURCES += previewbsa.cpp

HEADERS += previewbsa.h


include(../plugin_template.pri)

OTHER_FILES += \
    previewbsa.json\
    SConscript
