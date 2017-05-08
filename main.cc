#include "mainwindow.h"
#include <QApplication>
#include <qDebug.h>
#include <WebServerCore.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    BS::WebServerCore server;
    w.server = &server;


    return a.exec();
}
