#include "mainwindow.h"
#include "database.h"
#include "controllerclass.h"
#include "QScopedPointer"
#include <QApplication>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <future>

/*!
 * The main function creates the main window object to start showing the UI on program start-up.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}
