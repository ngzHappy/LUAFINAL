
CONFIG   += c++14
QT       += core
QT       -= gui

TARGET = QLuaCore
TEMPLATE = lib

DEFINES += QLUACORE_LIBRARY

SOURCES += QLuaCore.cpp \
    QLuaCoreDetail.cpp \
    PureLuaCore.cpp

HEADERS += $$PWD/QLuaCore.hpp \
    PureLuaCore.hpp
HEADERS += $$PWD/QLuaCoreDetail.hpp
HEADERS += $$PWD/QSTDSharedPointer.hpp
HEADERS += $$PWD/qluacore_global.hpp

# This one handles dllimport/dllexport directives.
DEFINES += QUAZIP_BUILD

# You'll need to define this one manually if using a build system other
# than qmake or using QuaZIP sources directly in your project.
# CONFIG(staticlib): DEFINES += QUAZIP_STATIC

include( $$PWD/QUAZIP/quazip.pri )
include( $$PWD/STDLIBS/stdlibsc14.pri )
include( $$PWD/LUASource/qlua.pri )
include( $$PWD/../BuildPaths.pri )

DESTDIR = $$BUILD_PATH_















