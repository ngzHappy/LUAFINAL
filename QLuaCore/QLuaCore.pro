
CONFIG   += c++14
QT       += core
QT       -= gui

TARGET = QLuaCore
TEMPLATE = lib

DEFINES += QLUACORE_LIBRARY

SOURCES += QLuaCore.cpp \
    QLuaCoreDetail.cpp

HEADERS += $$PWD/QLuaCore.hpp \
    QLuaCoreDetail.hpp \
    QSTDSharedPointer.hpp
HEADERS += $$PWD/qluacore_global.hpp

include( $$PWD/STDLIBS/stdlibsc14.pri )
include( $$PWD/LUASource/qlua.pri )
include( $$PWD/../BuildPaths.pri )

DESTDIR = $$BUILD_PATH_












