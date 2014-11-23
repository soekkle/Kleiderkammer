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

int SQLiteQuelle::addGroesse(QString Groesse, int Typ)
{
    QSqlQuery Abfrage("",Datenbank);
    Abfrage.exec(QString("SELECT * FROM Kleidungstyp WHERE id=%1").arg(Typ));
    if (Abfrage.next())
    {
        Abfrage.clear();
        Abfrage.exec(QString("INSERT INTO Groessen('Groesse','Typ') VALUES('%1',%2);").arg(Groesse).arg(Typ));
        return Abfrage.lastInsertId().toInt();
    }
    std::cerr<<"Größe \""<<Groesse.toStdString()<<"\"Kann nicht hinzugefügt werden.";
    return -1;
}

int SQLiteQuelle::addJugendfeuerwehr(QString Name)
{
    QSqlQuery Abfrage(QString("INSERT INTO Jugendfeuerwehr('Name') VALUES ('%1')").arg(Name),Datenbank);
    return Abfrage.lastInsertId().toInt();
}

/*!
 * \brief SQLiteQuelle::addKleiderstueck
 * \param Typ Typ des Kleidungsstück.
 * \param Groesse Größe des Kleidungsstücks
 * \param Nummer Gewünschte Nummer für das Kleidungsstück.
 * \return Die Nummer des Hinzugefügten Kleidungsstück.
 */
int SQLiteQuelle::addKleiderstueck(int Typ, int Groesse, int Nummer)
{
    QSqlQuery Abfrage("",Datenbank);
    QString SQLString;
    if (Nummer>0)
    {
        SQLString=QString("SELECT Nummer FROM Kleidungsstuecke WHERE Nummer=%1;").arg(Nummer);
        Abfrage.exec(SQLString);
        if(Abfrage.next())
        {
            Nummer=freieNummer(Typ);
        }
    }
    else
    {
        Nummer=freieNummer(Typ);
    }
    SQLString=QString("insert into Kleidungsstuecke ('Nummer','Typ','Groesse','Traeger') Values(%1,%2,%3,0);").arg(Nummer).arg(Typ).arg(Groesse);
    Abfrage.exec(SQLString);
    return Nummer;
}

int SQLiteQuelle::addKleidungstyp(QString Name,int AnfangsNummer,int Endnummer)
{
    QSqlQuery Abfrage(QString("INSERT INTO Kleidungstyp ('Name','AnNummer','EndNummer')Values('%1',%2,%3)").arg(Name).arg(AnfangsNummer).arg(Endnummer),Datenbank);
    return Abfrage.lastInsertId().toInt();
}

int SQLiteQuelle::addPerson(QString Nachname, QString Vorname,int Gruppe)
{
    QSqlQuery Abfrage("",Datenbank);
    QString SQLString=QString("insert into Personen ('Nachname','Vorname','JF') Values('%1','%2',%3);").arg(Nachname,Vorname).arg(Gruppe);
    Abfrage.exec(SQLString);
    return Abfrage.lastInsertId().toInt();
}

void SQLiteQuelle::createDB()
{
    std::clog<<"Die Tabellen werden in der Datenank erzeugt.\n";
    QSqlQuery Abfrage("",Datenbank);
    Abfrage.exec("create table Kleidungsstuecke(id integer primary key AUTOINCREMENT,Nummer integer,Typ integer,Groesse integer,Traeger integer)");
    Abfrage.exec("create table Kleidungstyp(id integer primary key AUTOINCREMENT,Name varchar,AnNummer integer,EndNummer integer)");
    Abfrage.exec("create table Personen(id integer primary key AUTOINCREMENT,Nachname varchar,Vorname varchar,Jf integer)");
    Abfrage.exec("create table Jugendfeuerwehr(id integer primary key AUTOINCREMENT,Name varchar)");
    Abfrage.exec("create table Groessen(id integer primary key AUTOINCREMENT, Groesse varchar,Typ integer)");
    QSqlError Fehler=Abfrage.lastError();
    std::cout<<Fehler.text().toStdString();
}

int SQLiteQuelle::freieNummer(int Typ)
{
    QSqlQuery Abfrage("",Datenbank);
    Abfrage.exec(QString("SELECT AnNummer,EndNummer FROM Kleidungstyp WHERE id=%1").arg(Typ));
    int min=0,max=0;
    if(Abfrage.next())
    {
        min=Abfrage.value(0).toInt();
        max=Abfrage.value(1).toInt();
    }
    Abfrage.clear();
    Abfrage.exec(QString("SELECT Nummer FROM Kleidungsstuecke WHERE Typ=%1 ORDER BY Nummer ASC").arg(Typ));
    if (Abfrage.next())
    {
        int Nummer=Abfrage.value(0).toInt();
        for (int i=min;i<max;i++)
        {
            while(Nummer<i)
            {
                if(Abfrage.next())
                {
                    Nummer=Abfrage.value(0).toInt();
                }
                else
                    return i;
            }
            if (Nummer>i)
                return i;
        }
    }
    return -1;
}
