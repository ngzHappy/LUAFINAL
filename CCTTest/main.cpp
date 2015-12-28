#include "MainWindow.hpp"
#include "StringTest.hpp"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    StringTest test;
    test.test();

    MainWindow windows;
    windows.show();

    return app.exec();
}



