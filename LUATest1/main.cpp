#include "MainWindow.hpp"
#include <QApplication>

#include <QLuaCore.hpp>

int wrap(lua_State * L) {
    {
        int i=0;
        do {
            lua_pushvalue(L,lua_upvalueindex(++i));
        } while (lua_type(L,-1)!=LUA_TNIL);
    }
    lua_pop(L,1);
    return lua_pcall(L,lua_gettop(L)-1,LUA_MULTRET,0);
}

int test1(lua_State * L) {
    lua_rotate(L,1,1);
    lua_pushcclosure(L,wrap,lua_gettop(L));
    return lua_pcall(L,0,LUA_MULTRET,0);
}

int test2(lua_State * L) {
   
    lua_pushvalue(L,1);
    lua_pushnumber(L,2);
    lua_pushcclosure(L,wrap,2);
 
    lua_pushnumber(L,11);
    lua_setupvalue(L,-2,2);

    return lua_pcall(L,0,LUA_MULTRET,0);
}
 
namespace cct{
class LuaFunction {
    std::shared_ptr<lua_State> L_;
    int max_args_=0;
    int this_index_=0;
    class NextNum {
        std::recursive_mutex mutex;
        typedef std::unique_lock<std::recursive_mutex> lock_t;
        std::set<int> unused;
        std::set<int> used;
        int max=0;
        ~NextNum()=default;
        NextNum():max(10){ for (int i=1; i<max; ++i) { unused.insert(i); } }
        NextNum(const  NextNum &)=delete;
        NextNum( NextNum &&)=delete;
        NextNum&operator=(const  NextNum &)=delete;
        NextNum&operator=( NextNum &&)=delete;
    public:
        static NextNum & instance() { static NextNum  * _this = new NextNum; return *_this; }
        int load() {
            lock_t _(mutex);
            if (unused.empty()==false) { auto ansp=unused.begin();
            auto ans=*ansp;unused.erase(ansp);
            used.insert(ans);return ans;
            }
            used.insert(max);return max++;
        }
        void store(int i) {
            lock_t _(mutex);
            used.erase(i); 
            unused.insert(i);
        }
    };
    
    static int __wrap(lua_State * L) {
        int i=0;
        do {
            lua_pushvalue(L,lua_upvalueindex(++i));
        } while (lua_type(L,-1)!=LUA_TNIL);
        lua_pop(L,1);
        auto ans = lua_pcall(L,lua_gettop(L)-1,LUA_MULTRET,0);
        const int top=lua_gettop(L);
        for (int j=2; j<i;++j) {
            lua_pushnil(L); lua_replace(L,lua_upvalueindex(j));
        }
        lua_settop(L,top);
        return ans;
    }
public:

    const std::shared_ptr<lua_State> & state() const{ return L_; }
    int maxArgs()const { return max_args_; }

    LuaFunction( 
        std::shared_ptr<lua_State> L,
        int function_index_in_lua_state,
        int max_args = 32)
        :L_(std::move(L)) 
        ,max_args_(max_args){
        if (max_args_>255) { max_args_=255; }
        if (max_args_<1) { max_args_=1; }
        auto * L__=L_.get();
        if (L__) {
            auto & thisIndex__ = NextNum::instance();
            this_index_=thisIndex__.load();
            function_index_in_lua_state=lua_absindex(L__,function_index_in_lua_state);
            const auto top__=lua_gettop(L__);
            auto top_=max_args_+top__;
            lua_settop(L__,top_);
            lua_pushcclosure(L__,&__wrap,max_args_);
            top_=lua_gettop(L__);/*set top_ to the wrap function*/
            luaL_newmetatable(L__,"@.@luatoc++function");
            lua_pushvalue(L__,top_);
            lua_rawseti(L__,-2,this_index_);

            lua_pushvalue(L__,function_index_in_lua_state);
            lua_setupvalue(L__,top_,1);

            lua_settop(L__,top__);
        }
        
    }

    ~LuaFunction() {
        auto * L__=L_.get();
        if (L__) {
            const auto top__=lua_gettop(L__);
            luaL_newmetatable(L__,"@.@luatoc++function");
            lua_pushnil(L__);lua_rawseti(L__,-2,this_index_);
            lua_settop(L__,top__);
            auto & thisIndex__ = NextNum::instance();
            thisIndex__.store( this_index_ );
        }
       
    }

    LuaFunction(const LuaFunction &)=delete;
    LuaFunction( LuaFunction &&)=default;
    LuaFunction&operator=(const LuaFunction &)=delete;
    LuaFunction&operator=( LuaFunction &&)=default;

    int operator()() const{
        auto * L__=L_.get();
        if (L__) {
            const auto top__=lua_gettop(L__);
            luaL_newmetatable(L__,"@.@luatoc++function");
            const auto call_pos_=lua_gettop(L__);
            lua_rawgeti(L__,-1,this_index_);
            if (lua_isfunction(L__,-1)==false) { return -3; }
            const auto function_=lua_gettop(L__);
           // bool temp_[]{ (cct::lua::pushValue(L_,std::forward<Args>(args)),true)... };
            lua_copy(L__,function_,call_pos_); 
            lua_settop(L__,call_pos_);
            return lua_pcall(L__,0,LUA_MULTRET,0);
        }
        return -1;
    }

    template<typename ... Args>
    int operator()( Args && ... args ) const{
        auto * L__=L_.get();

        if ((1+sizeof...(args))>max_args_) { return -2; }

        if (L__) {
            const auto top__=lua_gettop(L__);
            luaL_newmetatable(L__,"@.@luatoc++function");
            const auto call_pos_=lua_gettop(L__);
            lua_rawgeti(L__,-1,this_index_);
            if (lua_isfunction(L__,-1)==false) { return -3; }
            const auto function_=lua_gettop(L__);
            // bool temp_[]{ (cct::lua::pushValue(L_,std::forward<Args>(args)),true)... };

            {
                int current_index_=1;
                auto set_=[L__,&current_index_,function_](const auto & data_) mutable {
                    cct::lua::pushValue(L__,data_);
                    return lua_setupvalue(L__,function_,++current_index_) ;
                };
                const char * temp_[]={ set_(args)... }; (void)temp_;
            }

            lua_copy(L__,function_,call_pos_); 
            lua_settop(L__,call_pos_);
            return lua_pcall(L__,0,LUA_MULTRET,0);
        }
        return -1;
    }

};

namespace spr {
class LuaFunction :
    public std::shared_ptr<cct::LuaFunction>{
    typedef std::shared_ptr<cct::LuaFunction> Super;
    typedef cct::LuaFunction type_;
    const type_ & __get() const{ return *(this->get()); }
public:

    LuaFunction() {}
    LuaFunction(std::shared_ptr<lua_State> L,
        int function_index_in_lua_state,
        int max_args = 32): Super(std::make_shared<type_>(L,function_index_in_lua_state,max_args)){}
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
};
}
}

using namespace cct;
class NextNum {
    std::recursive_mutex mutex;
    typedef std::unique_lock<std::recursive_mutex> lock_t;
    std::set<int> unused;
    std::set<int> used;
    int max=0;
    ~NextNum()=default;
    NextNum():max(10){ for (int i=1; i<max; ++i) { unused.insert(i); } }
    NextNum(const  NextNum &)=delete;
    NextNum( NextNum &&)=delete;
    NextNum&operator=(const  NextNum &)=delete;
    NextNum&operator=( NextNum &&)=delete;
public:
    static NextNum & instance() { static NextNum  * _this = new NextNum; return *_this; }
    int load() {
        lock_t _(mutex);
        if (unused.empty()==false) { auto ansp=unused.begin();
        auto ans=*ansp;unused.erase(ansp);
        used.insert(ans);return ans;
    }
    used.insert(max);return max++;
    }
    void store(int i) {
        lock_t _(mutex);
        used.erase(i); 
        unused.insert(i);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    {
        auto & n = NextNum::instance();
        std::vector<int >v;
        for (int i=0; i<100;++i) {
            v.push_back( n.load() );
        }
    }

    {
        cct::PureLuaCore core;
        auto * L=core.getStatePointer();
        using namespace::cct::lua;
        //setGlobalValue(L,"test1",test1);
        //setGlobalValue(L,"test2",test2);
        //doString(L,"test1(1,2,3,function(x,y,z) print(x);print(y);print(z); end )");
        //doString(L," yy = function(x) print(x) end ");
        //doString(L,"test2( yy )");
        doString(L,"return function(x,y) print(x);print(y) ;print('Hellow Word!') end");
        LuaFunction function_( core.getLuaState(),-1 );
        function_("123",976.222);
        spr::LuaFunction ftest(std::move(function_));
        std::function<int(int)> ftest1=ftest;
        ftest1(777);
    }
    
    
    MainWindow windows;
    windows.show();

    return app.exec();
}
