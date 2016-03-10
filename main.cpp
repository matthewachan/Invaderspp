/** @file main.cpp
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @brief This is the main() function for the Space Invaders game
 */
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
