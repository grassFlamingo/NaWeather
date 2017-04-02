#include "mainwindow.h"
#include <QApplication>

#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName("Weather.db");

    MainWindow w;
    w.show();

    return a.exec();
}
