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
    std::cerr<<Abfrage.lastError().text().toStdString()<<std::endl;
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
    else
    {
        return -1;
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
    return min;
}

GroessenTabelle *SQLiteQuelle::getGroessen(int *Filter, int anz)
{
    GroessenTabelle *Ausgabe=new GroessenTabelle;
    Ausgabe->Anzahl=0;
    QString SQLString="SELECT Groessen.id, Groesse,name FROM Groessen,Kleidungstyp WHERE Groessen.Typ=Kleidungstyp.id";
    if (anz>0)
    {
        SQLString=SQLString.append(" AND ( Groessen.Typ= %1").arg(Filter[0]);
        for (int i=1;i<anz;i++)
        {
            SQLString=SQLString.append(" OR Groessen.Typ=%1").arg(Filter[i]);
        }
        SQLString=SQLString.append(" );");
    }
    std::cout<<SQLString.toStdString()<<std::endl;
    QSqlQuery Abfrage(SQLString,Datenbank);
    while(Abfrage.next())
    {
        ++Ausgabe->Anzahl;
        Ausgabe->IDs.append(Abfrage.value(0).toInt());
        Ausgabe->Namen.append(Abfrage.value(1).toString());
        Ausgabe->Typ.append(Abfrage.value(2).toString());
    }
    return Ausgabe;
}

JugendFeuerwehrTabelle *SQLiteQuelle::getJugendfeuerwehr()
{
    JugendFeuerwehrTabelle *Ausgabe=new JugendFeuerwehrTabelle;
    Ausgabe->Anzahl=0;
    QSqlQuery Abfrage("SELECT id,Name FROM Jugendfeuerwehr",Datenbank);
    while(Abfrage.next())
    {
        ++Ausgabe->Anzahl;
        Ausgabe->ID.append(Abfrage.value(0).toInt());
        Ausgabe->Name.append(Abfrage.value(1).toString());
    }
    return Ausgabe;
}

KleiderTabelle *SQLiteQuelle::getKleider(int Typ, int Groesse,int Traeger)
{
    KleiderTabelle *Ausgabe=new KleiderTabelle;
    Ausgabe->Anzahl=0;
    QString SQLString="SELECT Kleidungsstuecke.id,  Nummer, Groessen.Groesse , Kleidungstyp.Name FROM Kleidungsstuecke ,Groessen,Kleidungstyp WHERE Kleidungsstuecke.Groesse=Groessen.id AND Kleidungsstuecke.Typ=Kleidungstyp.id";
    if(Typ>0)
        SQLString=SQLString.append(" AND Kleidungsstuecke.Typ=%1").arg(Typ);
    if (Groesse>0)
        SQLString=SQLString.append(" AND Kleidungsstuecke.Groesse=%1").arg(Groesse);
    SQLString=SQLString.append(" AND Kleidungsstuecke.Traeger=%1").arg(Traeger);
    std::cout<<SQLString.toStdString()<<std::endl;
    QSqlQuery Abfrage(SQLString,Datenbank);
    while (Abfrage.next())
    {
        ++Ausgabe->Anzahl;
        Ausgabe->ID.append(Abfrage.value(0).toInt());
        Ausgabe->Nummer.append(Abfrage.value(1).toInt());
        Ausgabe->Groesse.append(Abfrage.value(2).toString());
        Ausgabe->Typ.append(Abfrage.value(3).toString());
    }
    std::cout<<Abfrage.lastQuery().toStdString()<<std::endl;
    std::cerr<<Abfrage.lastError().text().toStdString()<<std::endl;
    return Ausgabe;
}

KleiderTabelle *SQLiteQuelle::getKleiderinKammer(int Typ, int Groesse)
{
    return getKleider(Typ,Groesse,0);
}

KleiderTabelle *SQLiteQuelle::getKleidervonPerson(int id)
{
    return getKleider(-1,-1,id);
}

Kleidungstypentabelle *SQLiteQuelle::getKleidungstypen()
{
    Kleidungstypentabelle *Ausgabe=new Kleidungstypentabelle;
    QString SQLString="SELECT id, Name,AnNummer,Endnummer FROM Kleidungstyp";
    QSqlQuery Abfrage(SQLString,Datenbank);
    Ausgabe->Anzahl=0;
    while (Abfrage.next())
    {
        ++Ausgabe->Anzahl;
        Ausgabe->ID.append(Abfrage.value(0).toInt());
        Ausgabe->Name.append(Abfrage.value(1).toString());
        Ausgabe->AnfangsNummer.append(Abfrage.value(2).toInt());
        Ausgabe->EndNummer.append(Abfrage.value(3).toInt());
    }
    return Ausgabe;
}

int SQLiteQuelle::getIDByKleidungsNummer(int Nummer)
{
    QSqlQuery Abfrage(QString("SELECT id FROM Kleidungsstuecke WHERE Nummer=%1").arg(Nummer),Datenbank);
    if(!Abfrage.next())
        return -1;
    return Abfrage.value(0).toInt();
}

void SQLiteQuelle::getNummerBereich(int TypID, int *Anfang, int *Ende)
{
    QSqlQuery Abfrage(QString("SELECT AnNummer,EndNummer FROM Kleidungstyp WHERE id=%1").arg(TypID),Datenbank);
    if (Abfrage.next())
    {
        *Anfang=Abfrage.value(0).toInt();
        *Ende=Abfrage.value(1).toInt();
    }
}

bool SQLiteQuelle::KleidungsstueckzuordnenbyID(int ID, int Traeger)
{
    QSqlQuery Abfrage(QString("SELECT Traeger FROM Kleidungsstuecke WHERE id=%1").arg(ID),Datenbank);
    if(Abfrage.next())
    {
        Abfrage.clear();
        Abfrage.exec(QString("UPDATE Kleidungsstuecke SET 'Traeger'=%1 WHERE id=%2").arg(Traeger).arg(ID));
        std::cerr<<Abfrage.lastError().text().toStdString()<<std::endl;
        return true;
    }
    return false;
}

PersonenTabelle *SQLiteQuelle::getPersonen(int *JFFilter, int JFans)
{
    PersonenTabelle *Ausgabe=new PersonenTabelle;
    Ausgabe->Anzahl=0;
    QString SQLString="SELECT Personen.id,Personen.Nachname,Personen.Vorname,Jugendfeuerwehr.Name FROM Personen,Jugendfeuerwehr WHERE Personen.jf=Jugendfeuerwehr.id";
    if (JFans>0)
    {
        SQLString=SQLString.append(" AND (Personen.jf=%1").arg(JFFilter[0]);
        for (int i=1;i<JFans;++i)
        {
            SQLString=SQLString.append(" OR Personen.jf=%1").arg(JFFilter[i]);
        }
        SQLString=SQLString.append(")");
    }
    QSqlQuery Abfrage(SQLString,Datenbank);
    while(Abfrage.next())
    {
        ++Ausgabe->Anzahl;
        Ausgabe->ID.append(Abfrage.value(0).toInt());
        Ausgabe->Nachname.append(Abfrage.value(1).toString());
        Ausgabe->Vorname.append(Abfrage.value(2).toString());
        Ausgabe->JugendFeuerwehr.append(Abfrage.value(3).toString());
    }
    return Ausgabe;
}

bool SQLiteQuelle::removeGrosse(int ID)
{
    QSqlQuery Abfrage(QString("DELETE FROM Grosse WHERE id=%1").arg(ID),Datenbank);
    return true;
}

bool SQLiteQuelle::removeJugendferweher(int ID)
{
    QSqlQuery Abfrage(QString("DELETE Jugendfeuerweher WHERE id=%1").arg(ID),Datenbank);
    return true;
}

bool SQLiteQuelle::removeKleidungstyp(int ID)
{
    QSqlQuery Abfrage(QString("DELETE FROM Kleidungstyp WHERE id=%1").arg(ID),Datenbank);
    return true;
}

bool SQLiteQuelle::removePerson(int ID)
{
    QSqlQuery Abfrage(QString("DELETE FROM Personen WHERE id=%1").arg(ID),Datenbank);
    return true;
}

void SQLiteQuelle::rueckgabeKleidungsstueck(int ID)
{
    QSqlQuery Abfrage(QString("UPDATE Kleidungsstuecke SET 'Traeger'=0 WHERE id=%1").arg(ID),Datenbank);
}
