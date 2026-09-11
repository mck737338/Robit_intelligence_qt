#include "mainwindow.h"
#include <iostream>
#include <unistd.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    std::cout << "start";
    for(int i = 0; i < 3; i++){
        sleep(1);
        std::cout << ".";
    }
    std::cout << "\n";
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
