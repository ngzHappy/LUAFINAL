#if !defined(__LUATABLE_HPP__CCT)
#define __LUATABLE_HPP__CCT

#include "PureLuaCore.hpp"

namespace cct {

class QLUACORESHARED_EXPORT LUATable {
    LUATable(const LUATable &)=delete;
    LUATable&operator=(const LUATable &)=delete;
    constexpr static const char * __metaTableName() { return "@.@luatoc++table"; }
    std::shared_ptr<lua_State> L__;
    int this_index_=0;
public:
    LUATable(std::shared_ptr<lua_State> L_,int table_in_lua_state_);
    ~LUATable();

    LUATable(LUATable &&)=default;
    LUATable&operator=(LUATable &&)=default;

    void pushTable()const;
    void push() const { pushTable(); }

    const std::shared_ptr<lua_State> & state() const { return L__; }
};

typedef LUATable LuaTable;

}

namespace cct {
namespace spr {
class QLUACORESHARED_EXPORT LUATable:
    public std::shared_ptr<cct::LUATable> {
    typedef std::shared_ptr<cct::LUATable> Super;
    typedef cct::LUATable type_;
public:
    LUATable() {}
    LUATable(decltype(nullptr)) {}
    LUATable(std::shared_ptr<lua_State> L_,int table_in_lua_state_):
        Super( std::make_shared<type_>( std::move(L_) ,table_in_lua_state_) ) {}
    LUATable(type_ && t):Super( new type_(std::move(t)) ) {}
    LUATable(Super && s):Super(std::move(s)) {}
    LUATable(const Super & s):Super(s) {}
    LUATable(const LUATable &)=default;
    LUATable(LUATable &&)=default;
    LUATable&operator=(const LUATable &)=default;
    LUATable&operator=(LUATable &&)=default;
};
typedef LUATable LuaTable;
}
}


#endif // LUATABLE_HPP





