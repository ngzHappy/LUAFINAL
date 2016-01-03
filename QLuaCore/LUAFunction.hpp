#if !defined(__LUAFUNCTION_HPP__CCT)
#define __LUAFUNCTION_HPP__CCT

#include "PureLuaCore.hpp"

namespace cct{
class QLUACORESHARED_EXPORT LuaFunction {
    std::shared_ptr<lua_State> L_;
    int max_args_=0;
    int this_index_=0;
    constexpr static const char * __metaTableName() { return "@.@luatoc++function"; }
    static int __wrap(lua_State * L);
public:

    const std::shared_ptr<lua_State> & state() const{ return L_; }
    int maxArgs()const { return max_args_; }

    LuaFunction(std::shared_ptr<lua_State> L,int function_index_in_lua_state,int max_args=32);
    ~LuaFunction();

    LuaFunction(const LuaFunction &)=delete;
    LuaFunction( LuaFunction &&)=default;
    LuaFunction&operator=(const LuaFunction &)=delete;
    LuaFunction&operator=( LuaFunction &&)=default;

    void pushFunction()const;
    int operator()() const;
    template<typename ... Args>int operator()(Args && ... args) const;

};
typedef LuaFunction LUAFunction;

namespace spr {
class QLUACORESHARED_EXPORT LuaFunction :
    public std::shared_ptr<cct::LuaFunction>{
    class  QLUACORESHARED_EXPORT LUAFUNCTIONINFO {public:
        LUAFUNCTIONINFO();
        ~LUAFUNCTIONINFO();
    };
    static class LUAFUNCTIONINFO luafunctioninfo_;
    typedef std::shared_ptr<cct::LuaFunction> Super;
    typedef cct::LuaFunction type_;
    const type_ & __get() const{ return *(this->get()); }
public:

    LuaFunction()=default;
    LuaFunction(decltype(nullptr)) {}
    LuaFunction(std::shared_ptr<lua_State> L,
        int function_index_in_lua_state,
        int max_args = 32): Super(std::make_shared<type_>( std::move(L),function_index_in_lua_state,max_args)){}
    LuaFunction(Super && s):Super(std::move(s)) {}
    LuaFunction(const Super & s):Super(s){}
    LuaFunction(type_ && t):Super( new type_(std::move(t)) ) {}
    LuaFunction(const LuaFunction &)=default;
    LuaFunction(LuaFunction &&)=default;
    LuaFunction&operator=(const LuaFunction &)=default;
    LuaFunction&operator=(LuaFunction &&)=default;

    int operator()() const { return (this->__get())(); }
    template<typename ... Args>
    int operator()(Args && ... args) {return (this->__get())(std::forward<Args>(args)...);}

    void pushFunction()const { return (this->__get()).pushFunction(); }
};
typedef LuaFunction LUAFunction;
}
}

#endif // LUAFUNCTION_HPP


