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

        doString(L,"return function(x,y) print(y);print(x) ;print('Hellow Word!') end");
        LuaFunction function_1( core.getLuaState(),-1 );
        
        function_();
        function_("123",976.222);
        function_1("x","y");
        spr::LuaFunction ftest(std::move(function_));
        ftest.pushFunction();
        lua_pushinteger(L,1); lua_pushinteger(L,2);
        lua_pcall(L,2,LUA_MULTRET,0);
        std::function<int(int)> ftest1=ftest;
        ftest1(777);
        pushValue(L,ftest);
        lua_pushinteger(L,5); lua_pushinteger(L,6);
        call(L,2);
        QVariant v=QVariant::fromValue( ftest );
        pushValue(L,v);
        lua_pushinteger(L,7); lua_pushinteger(L,8);
        call(L,2);
    }


    MainWindow windows;
    windows.show();

    return app.exec();
}
