#include "MainWindow.hpp"
#include <QApplication>


const char testFile[]=u8R"==(
t0=1
t1={a=11,b=12,c=13,[0]=3.1,test=99.5}
)==";

#include <QLuaCore.hpp>

void test(bool v,const QString & what = "") {
    if (v==false) { throw what; }
    qDebug()<<"pass "<<what;
}

int main(int argc, char *argv[])try
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

        test(configFile.read({ "t0" })==QVariant(1),"0");
        test(configFile.read({ "t1","a" }).toInt()==11,"1");
        test(std::abs(configFile.read({ "t1",0 }).toDouble()-3.1)<0.00001,"2");

        qDebug()<<"**********";
        configFile.write({ "t0" },"[==[ss");
        qDebug()<<configFile.read({ "t0" });

        test(configFile.read({ "t0" })=="[==[ss","3");

        configFile.write({ "t1","a" },1.0/3);
        qDebug()<<configFile.read({ "t1","a" });

        test(std::abs(configFile.read({ "t1","a" }).toDouble()-1/3.0)<0.00001,"4");

        configFile.write({ "t1",0 },5);
        qDebug()<<configFile.read({ "t1",0 });

        test(configFile.read({"t1",0})==5,"5");

        configFile.write({ "t2","a","b","c" },13);
        qDebug()<<configFile.read({ "t2","a","b","c" });

        test(configFile.read({"t2","a","b","c"})==13,"6");

        configFile.write({ "t2","a","b","c" },15);
        test(configFile.read({"t2","a","b","c"})==15,"7");

        configFile.save("t1.lua","t1");
        configFile.save("t0.lua","t0");
    }

    {
        qDebug()<<"*****";
        LuaConfigFile configFile;
        configFile.load("t0.lua");
        configFile.load("t1.lua");
        qDebug()<<configFile.read({ "t1","a" });
        test(std::abs(configFile.read({ "t1","a" }).toDouble()-1/3.0)<0.00001,"8");
        qDebug()<<configFile.read(std::vector<QVariant>{ "t0" });
        test(configFile.read({ "t0" })=="[==[ss","9");

    }

    MainWindow win;
    win.show();

    return app.exec();
}
catch (const QString & v) {
    qDebug()<<v;
    throw;

}
