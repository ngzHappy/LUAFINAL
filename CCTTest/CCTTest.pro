
CONFIG += c++14
CONFIG += console

QT += core gui
QT += widgets

TARGET = CCTTest
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    StringTest.cpp

HEADERS  += $$PWD/MainWindow.hpp \
    StringTest.hpp

INCLUDEPATH += $$PWD/../QLuaCore

include( $$PWD/../BuildPaths.pri )
DESTDIR = $$BUILD_PATH_
LIBS += -L$$BUILD_PATH_ -lQLuaCore



