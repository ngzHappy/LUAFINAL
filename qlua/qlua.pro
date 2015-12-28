
CONFIG += c++14
CONFIG += console

QT += core
QT -= gui

TARGET = qlua
TEMPLATE = app

SOURCES += $$PWD/main.cpp

HEADERS  +=

INCLUDEPATH += $$PWD/../QLuaCore
INCLUDEPATH += $$PWD/../QLuaCore/LUASource

include( $$PWD/../BuildPaths.pri )
DESTDIR = $$BUILD_PATH_
LIBS += -L$$BUILD_PATH_ -lQLuaCore



