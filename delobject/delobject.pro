#-------------------------------------------------
#
# Project created by QtCreator 2011-03-22T19:33:11
#
#-------------------------------------------------

QT       += gui
TEMPLATE = lib
CONFIG += plugin
VERSION = 1.0.1
PLUGIN_NAME=delobject

GENERATED_DIR = ../../generated/plugin/delobject
# Use common project definitions.
include(../../common.pri)

# For plugins
INCLUDEPATH    += ../../librecad/src/plugins

SOURCES += delobject.cpp

HEADERS += delobject.h


# DLLDESTDIR = ../../unix/resources/plugins/
win32 {
        DLLDESTDIR = ../../windows/resources/plugins
        TARGET = $$PLUGIN_NAME
}
unix {
    macx { 
	TARGET = ../../LibreCAD.app/Contents/Resources/plugins/$$PLUGIN_NAME
    }
    else { 
	TARGET = ../../unix/resources/plugins/$$PLUGIN_NAME
    }
}
