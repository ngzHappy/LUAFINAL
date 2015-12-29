#if !defined(QLUACORE_HPP__)
#define QLUACORE_HPP__

#include "qluacore_global.hpp"
#include "LUASource/lua.hpp"
#include "QLuaCoreDetail.hpp"
#include "QSTDSharedPointer.hpp"
#include "STDLIBS/STDLIBS.hpp"
#include "QUAZIP/quazip.h"
#include "QUAZIP/quazipfile.h"

class QLUACORESHARED_EXPORT QLuaCore
{

public:
    QLuaCore();
    virtual ~QLuaCore();
};

#endif // QLUACORE_HPP




