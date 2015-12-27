
CONFIG   += c++14
QT       += core
QT       -= gui

TARGET = QLuaCore
TEMPLATE = lib

DEFINES += QLUACORE_LIBRARY

SOURCES += QLuaCore.cpp

HEADERS += $$PWD/QLuaCore.hpp
HEADERS += $$PWD/qluacore_global.hpp

include( $$PWD/LUASource/qlua.pri )
include( $$PWD/../BuildPaths.pri )

HEADERS += $$PWD/VirtualBase.hpp
DESTDIR = $$BUILD_PATH_












