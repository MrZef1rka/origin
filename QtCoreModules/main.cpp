#include <QCoreApplication>
#include <QAuthenticator>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QAuthenticator authenticator;
    QSqlQuery query;
    return a.exec();
}
