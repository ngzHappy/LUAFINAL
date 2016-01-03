#include "MainWindow.hpp"
#include <QApplication>


namespace cct {}


int main(int argc, char *argv[]){
    QApplication app(argc, argv);





    MainWindow window;
    window.show();

    return app.exec();
}
