#include "MainWindow.hpp"
#include <QApplication>

#include <PureLuaCore.hpp>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    {
        cct::PureLuaCore core;
        auto sL=core.getLuaState();
        auto L=sL.get();
        lua_newtable(L);
        cct::LUATable table(sL,-1);

    }


    MainWindow window;
    window.show();

    return app.exec();
}
