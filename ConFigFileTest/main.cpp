#include "MainWindow.hpp"
#include <QApplication>


const char testFile[]=u8R"==(
t0=1
t1={a=11,b=12,c=13,[0]=3.1,test=99.5}
)==";

#include <QLuaCore.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    {
        std::ofstream ofs("test.lua");
        ofs<<testFile<<std::endl;
    }

    {
        LuaConfigFile configFile;
        configFile.load("test.lua");
        qDebug()<<configFile.read({ "t0" });
        qDebug()<<configFile.read({ "t1","a" });
        qDebug()<<configFile.read({ "t1",0 });

        qDebug()<<"**********";
        configFile.write({ "t0" },"[==[ss");
        qDebug()<<configFile.read({ "t0" });

        configFile.write({ "t1","a" },1.0/3);
        qDebug()<<configFile.read({ "t1","a" });

        configFile.write({ "t1","m" },5);
        qDebug()<<configFile.read({ "t1","m" });

        configFile.write({ "t2","a","b","c" },13);
        qDebug()<<configFile.read({ "t2","a","b","c" });

        configFile.save("t1.lua","t1");
        configFile.save("t0.lua","t0");
    }

    {
        qDebug()<<"*****";
        LuaConfigFile configFile;
        configFile.load("t0.lua");
        configFile.load("t1.lua");
        qDebug()<<configFile.read({ "t1","a" });
        qDebug()<<configFile.read({ "t0" });
    }

    MainWindow win;
    win.show();

    return app.exec();
}
