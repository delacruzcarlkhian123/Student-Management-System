#include "stdafx.h"
#include "loginwindows.h"
#include <QtWidgets/QApplication>
#include "centralCode.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("sms_ui");
    db.setUserName("root");
    db.setPassword("khian2004");

    if (db.open())
    {
        qDebug() << "Connected to MySQL successfully!";
    }
    else
    {
        qDebug() << "MySQL connection failed:";
        qDebug() << db.lastError().text();
    }

    loginWindows window;
    window.show();

    return app.exec();
}
