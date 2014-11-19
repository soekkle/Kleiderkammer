#include "sqlitequelle.h"

SQLiteQuelle::SQLiteQuelle(QString Pfad)
{
    Datenbank=QSqlDatabase::addDatabase("QSQLITE");
    Datenbank.setDatabaseName(Pfad);
    if (!Datenbank.open())
    {
        QSqlError Fehler=Datenbank.lastError();
        std::cerr <<Fehler.text().toStdString();
        return;
    }
    QStringList Tabellen=Datenbank.tables();
    if(Tabellen.length()==0)
    {
        std::clog<<"Leere Datenbank\n";
        createDB();
    }

}

void SQLiteQuelle::createDB()
{
    std::clog<<"Die Tabellen werden in der Datenank erzeugt.\n";
    QSqlQuery Abfrage("",Datenbank);
    Abfrage.exec("create table Kleidungsstuecke(id integer primary key,Nummer integer,Typ integer,Groesse varchar,Traeger integer)");

}
