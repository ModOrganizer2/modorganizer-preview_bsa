#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T19:50:56
#
#-------------------------------------------------

TARGET = previewBase
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

contains(QT_VERSION, "^5.*") {
	QT += widgets
}

DEFINES += PREVIEWBASE_LIBRARY

SOURCES += previewbase.cpp

HEADERS += previewbase.h


include(../plugin_template.pri)

OTHER_FILES += \
    previewbase.json
