#include "MainWindow.hpp"
#include <QApplication>


#include <QLuaCore.hpp>


void test(bool v,std::string e) { if (v==false) { throw e; } }

int main(int argc, char *argv[])try{

    QApplication app(argc, argv);

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









