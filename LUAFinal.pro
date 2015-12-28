TEMPLATE = subdirs

#.subdir	Use the specified subdirectory instead of SUBDIRS value.
#.file	Specify the subproject pro file explicitly. Cannot be used in conjunction with .subdir modifier.
#.depends	This subproject depends on specified subproject. Available only on platforms that use makefiles.
#.makefile	The makefile of subproject. Available only on platforms that use makefiles.
#.target	Base string used for makefile targets related to this subproject. Available only on platforms that use makefiles.

QLuaCore.file = $$PWD/QLuaCore/QLuaCore.pro
ConFigFileTest.file = $$PWD/ConFigFileTest/ConFigFileTest.pro
lua.file = $$PWD/lua/lua.pro

ConFigFileTest.depends += QLuaCore
lua.depends += QLuaCore

SUBDIRS += QLuaCore
SUBDIRS += lua
SUBDIRS += ConFigFileTest
