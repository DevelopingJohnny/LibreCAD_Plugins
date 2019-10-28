#-------------------------------------------------
# From Sample Plugin
# Project created by QtCreator 2011-03-22T19:33:11
#
#-------------------------------------------------

QT       += gui
TEMPLATE = lib
CONFIG += plugin
VERSION = 1.0.1
PLUGIN_NAME=boxjoints

GENERATED_DIR = ../../generated/plugin/boxjoints
# Use common project definitions.
include(../../common.pri)

# For plugins
INCLUDEPATH    += ../../librecad/src/plugins

SOURCES += boxjoints.cpp \
    bj_window.cpp \
    bj_generate.cpp

HEADERS += boxjoints.h \
    bj_window.h \
    bj_generate.h


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

FORMS +=
