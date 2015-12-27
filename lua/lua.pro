
CONFIG += c++14
CONFIG += console

QT += core
QT -= gui

TARGET = lua
TEMPLATE = app

SOURCES += $$PWD/../QLuaCore/LUASource/lua.cpp

HEADERS  +=

INCLUDEPATH += $$PWD/../QLuaCore

include( $$PWD/../BuildPaths.pri )
DESTDIR = $$BUILD_PATH_
LIBS += -L$$BUILD_PATH_ -lQLuaCore



