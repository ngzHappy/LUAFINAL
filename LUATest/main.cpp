#include "MainWindow.hpp"
#include <QApplication>


#include <QLuaCore.hpp>

void test(bool v,std::string e) { if (v==false) { throw e; } }

int callWrap0(lua_State * L) {
    const auto table=lua_gettop(L);
    lua_pushlstring(L,"function_",9);
    lua_gettable(L,table);
    return lua_pcall(L,0,LUA_MULTRET,0);
}

int callWrap1(lua_State * L) {
    const auto table=lua_gettop(L);
    lua_pushlstring(L,"function_",9);
    lua_gettable(L,table);
    lua_pushlstring(L,"01",2);
    lua_gettable(L,table);
    return lua_pcall(L,1,LUA_MULTRET,0);
}

int callWrap2(lua_State * L) {
    const auto table=lua_gettop(L);
    lua_pushlstring(L,"function_",9);
    lua_gettable(L,table);

    lua_pushlstring(L,"01",2);
    lua_gettable(L,table);
    
    std::cout<<lua_tonumber(L,-1)<<std::endl;

    lua_pushlstring(L,"02",2);
    lua_gettable(L,table);

    std::cout<<lua_tonumber(L,-1)<<std::endl;
    
    return lua_pcall(L,2,LUA_MULTRET,0);
}

#include <map>
class Integer { public:
    int value=0;
    Integer & operator ++() { ++value; return *this; }
    operator bool() const { return value!=0; }
};

std::map<lua_State *,Integer> xxxx;

int testLuaFunction(lua_State * L) {
    /*1 num*/  
    /*2 num*/ 
    /*3 fun*/
    lua_newtable(L);

    std::cout<<lua_gettop(L)<<std::endl;
    
    lua_pushlstring(L,"function_",9);
    lua_pushvalue(L,3);
    lua_settable(L,-3);

    lua_pushlstring(L,"01",2);
    lua_pushvalue(L,1);
    lua_settable(L,-3);

    lua_pushlstring(L,"02",2);
    lua_pushvalue(L,2);
    lua_settable(L,-3);

    lua_pushlstring(L,"wrap_",5);
    lua_pushcfunction(L,callWrap2);
    lua_settable(L,-3);

    lua_pushcfunction(L,callWrap2);
    lua_pushvalue(L,-2);
    
    ++xxxx[L];

    lua_add_state_counter(L);
    std::shared_ptr<lua_State> SL_(L,[](lua_State * SSLL_) {
        if (lua_sub_state_counter(SSLL_)) { return; }
        lua_close(SSLL_);
    });

    return lua_pcall(L,1,LUA_MULTRET,0);

}

int testLuaFunction1(lua_State * L) {
    std::cout<<lua_tonumber(L,1)<<","<<lua_tonumber(L,2)<<","<<lua_tonumber(L,3)<<std::endl;
    lua_rotate(L,1,1);
    std::cout<<lua_tonumber(L,1)<<","<<lua_tonumber(L,2)<<","<<lua_tonumber(L,3)<<std::endl;
    return 0;
}



int main(int argc, char *argv[])try{

    QApplication app(argc, argv);

    {
        cct::PureLuaCore core;
        auto * L = core.getStatePointer();
        std::string & errorString = core.getError();
        using namespace cct::lua;

        auto test1=[](lua_State * ) ->int {
            std::cout<<"Hellow Word!"<<std::endl;
            return 0;
        };

        setGlobalValue(L,"test1",test1);
        doString(L,"test1()");

        std::vector<std::string> keys1{"a"};
        std::list<std::string> keys2{"b","c"};
        std::deque<std::string> keys3{"d","e","f"};
        std::deque<std::string> keys4{"d","e","m","g"};

        setGlobals(L,keys1.begin(),keys1.end(),1,errorString);
        setGlobals(L,keys2.begin(),keys2.end(),2,errorString);
        setGlobals(L,keys3.begin(),keys3.end(),3,errorString);
        setGlobals(L,keys4.begin(),keys4.end(),4,errorString);

        getGlobals(L,keys1.begin(),keys1.end(),errorString);
        qDebug()<< toQVariant(L);
        getGlobals(L,keys2.begin(),keys2.end(),errorString);
        qDebug()<< toQVariant(L);
        getGlobals(L,keys3.begin(),keys3.end(),errorString);
        qDebug()<< toQVariant(L);
        getGlobals(L,keys4.begin(),keys4.end(),errorString);
        qDebug()<< toQVariant(L);

    }

    {
        cct::PureLuaCore core;
        core.pushValue(testLuaFunction1);
        core.setGlobal("test");
        core.doString("test(1,2,3)");
    }

    {
        cct::PureLuaCore core;
        std::cout<<core.getStatePointer()<<std::endl;
        auto L = core.getStatePointer();
        luaL_newmetatable(L,"@.@thread_function");
        core.pushValue(testLuaFunction);
        core.setGlobal("test");
        core.doString("yy = function(x,y) print(x+y) end");
        core.doString(" test(11,22,yy) ");
        qDebug()<< core.toQVariant();
    }

    {
        cct::PureLuaCore core;
        auto lock=core.getStateLock();
        core.doString("xx = 1+2");
        core.getGlobal("xx");
        qDebug()<<core.toQVariant();
    }

    {
        cct::PureLuaCore core;
        {
            auto lock=core.getStateLock();
            core.doString("xx=1+2");
            qDebug()<<"with out return"<<core.getTop();
        }
        core.doString("return 1+2");
        qDebug()<<"with return"<<core.getTop();
    }

    {
        cct::PureLuaCore core;
        auto lock=core.getStateLock();
        if (core.doString("xx = 1..+..2")) {
        }
        else {
            qDebug()<<core.toQVariant();
        }
    }

    {
        cct::PureLuaCore core;
        core.doString("xx = function() return a.f end");
        if ( core.doString(" xx() ") ) {
        }
        else {
            qDebug()<<core.getErrorQString();
        }
    }

    {
        cct::PureLuaCore core;

        auto * L = core.getLuaState().get();

        const char v[4]="123";
        const char * vxx="123";
        core.pushValue(vxx);
        core.pushValue(v);
        cct::PureLuaCore::pushValue(L,v);
        core.setTop();

        core.setGlobals({ "aa","bb",12 },33.2);
        core.getGlobals({ "aa","bb",12 });

        qDebug()<<core.toQVariant(-1);
        qDebug()<<core.getTop();

        test(core.toQVariant(-1)==33.2,"0");
        test(core.getTop()==1,"1");

        core.setTop();

        {
            std::vector< std::string > test1{"m12"};
            core.setGlobals(test1.begin(),test1.end(),"12");
            test( core.getGlobals(test1.begin(),test1.end() )== cct::PureLuaCore::LUAType::STRING ,"0 type");
            qDebug()<< core.toQVariant();

            test(core.toQVariant(-1)=="12","2");
            test(core.getTop()==1,"3");
        }

        core.setTop();

        {
            std::forward_list<std::string> test2({ { "m22" },{ "dd" } });
            core.setGlobals(test2.begin(),test2.end(),"77");
            test( core.getGlobals(test2.begin(),test2.end() )== cct::PureLuaCore::LUAType::STRING ,"q type");
            qDebug()<< core.toQVariant();
            test(core.toQVariant(-1)=="77","4");
            test(core.getTop()==1,"5");
        }


    }

    MainWindow window;
    window.show();

    return app.exec();
}catch(std::string & e) {
    std::cout<<e<<std::endl;
}









