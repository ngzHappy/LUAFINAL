TEMPLATE = subdirs

#.subdir	Use the specified subdirectory instead of SUBDIRS value.
#.file	Specify the subproject pro file explicitly. Cannot be used in conjunction with .subdir modifier.
#.depends	This subproject depends on specified subproject. Available only on platforms that use makefiles.
#.makefile	The makefile of subproject. Available only on platforms that use makefiles.
#.target	Base string used for makefile targets related to this subproject. Available only on platforms that use makefiles.

QLuaCore.file = $$PWD/QLuaCore/QLuaCore.pro
ConFigFileTest.file = $$PWD/ConFigFileTest/ConFigFileTest.pro
lua.file = $$PWD/lua/lua.pro
qlua.file = $$PWD/qlua/qlua.pro
CCTTest.file = $$PWD/CCTTest/CCTTest.pro
LUATest.file = $$PWD/LUATest/LUATest.pro
LUATest1.file = $$PWD/LUATest1/LUATest1.pro
LUATest2.file = $$PWD/LUATest2/LUATest2.pro

ConFigFileTest.depends += QLuaCore
lua.depends += QLuaCore
qlua.depends += QLuaCore
CCTTest.depends += QLuaCore
LUATest1.depends += QLuaCore
LUATest2.depends += QLuaCore
LUATest.depends += QLuaCore

SUBDIRS += QLuaCore
SUBDIRS += LUATest2
SUBDIRS += LUATest1
SUBDIRS += LUATest
SUBDIRS += CCTTest
SUBDIRS += qlua
SUBDIRS += lua
SUBDIRS += ConFigFileTest






























