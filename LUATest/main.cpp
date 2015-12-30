﻿#include "MainWindow.hpp"
#include <QApplication>


#include <QLuaCore.hpp>


void test(bool v,std::string e) { if (v==false) { throw e; } }

int main(int argc, char *argv[])try{

    QApplication app(argc, argv);

    {
        cct::PureLuaCore core;
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









