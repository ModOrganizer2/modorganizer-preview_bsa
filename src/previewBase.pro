#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T19:50:56
#
#-------------------------------------------------

TARGET = previewBase
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets gui

DEFINES += PREVIEWBASE_LIBRARY

SOURCES += previewbase.cpp

HEADERS += previewbase.h


include(../plugin_template.pri)

OTHER_FILES += \
    previewbase.json\
    SConscript
