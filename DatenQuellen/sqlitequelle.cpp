/*
 * Copyright (C) 2014-2017 Sören Krecker
 *
 * This file is part of Kleiderkammer.
 *
 * Kleiderkammer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation version 3 of the License.
 *
 * Kleiderkammer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Kleiderkammer.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Diese Datei ist Teil von Fubar.
 *
 * Kleiderkammer ist Freie Software: Sie können es unter den Bedingungen
 * der GNU Affero General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz, weiterverbreiten und/oder modifizieren.
 *
 * Kleiderkammer wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN
 * ZWECK.Siehe die GNU Affero General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU Affero General Public License zusammen mit
 * diesem Programm erhalten haben. Wenn nicht, siehe
 * <http://www.gnu.org/licenses/>.
*/

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
    FehlerAusgabe(Abfrage);
    if (Abfrage.next())
    {
        Abfrage.clear();
        Abfrage.exec(QString("INSERT INTO Groessen('Groesse','Typ') VALUES('%1',%2);").arg(Groesse).arg(Typ));
        return Abfrage.lastInsertId().toInt();
    }
    std::cerr<<"Größe \""<<Groesse.toStdString()<<"\"kann nicht hinzugefügt werden.";
    return -1;
}

int SQLiteQuelle::addJugendfeuerwehr(QString Name)
{
    QSqlQuery Abfrage(QString("INSERT INTO Jugendfeuerwehr('Name') VALUES ('%1')").arg(Name),Datenbank);
    return Abfrage.lastInsertId().toInt();
}

/*!
 * \brief SQLiteQuelle::addKleiderstueck
 * Fügt der Datenbank ein Kleidungstück hinzu.
 * \param Typ Typ des Kleidungsstück.
 * \param Groesse Größe des Kleidungsstücks
 * \param Nummer Gewünschte Nummer für das Kleidungsstück.
 * \return Die Nummer des Hinzugefügten Kleidungsstück.
 */
int SQLiteQuelle::addKleiderstueck(int Typ, int Groesse, int Nummer, bool uberschreibe)
{
    QSqlQuery Abfrage("",Datenbank);
    QString SQLString;
    if (Groesse<0)
        return -1;
    if (Nummer>0)//Pütt ob einen Nummer angegeben wurde
    {
        //Prüft ob die Nummer schon vergeben ist.
        SQLString=QString("SELECT Nummer FROM Kleidungsstuecke WHERE Nummer=%1;").arg(Nummer);
        Abfrage.exec(SQLString);
        if(Abfrage.next())
        {
            if (!uberschreibe)
                return -1;
            //Wenn das der Fall ist so wird die nächsste freie Nummer verwendet.
            Nummer=freieNummer(Typ);
        }
    }
    else
    {
        Nummer=freieNummer(Typ);
    }
    Abfrage.prepare("insert into Kleidungsstuecke ('Nummer','Typ','Groesse','DatumHin','DatumLeihe') Values(:Nummer,:Typ,:Groesse,:Datum,:DatumLeih);");
    Abfrage.bindValue(0, Nummer);//Einsetzen der Daten
    Abfrage.bindValue(1, Typ);
    Abfrage.bindValue(2,Groesse);
    Abfrage.bindValue(3,QDateTime::currentDateTimeUtc());
    Abfrage.bindValue(4,QDateTime::currentDateTimeUtc());
    Abfrage.exec();//Ausführen der Abfrage.
    //std::cerr<<Abfrage.lastError().text().toStdString()<<std::endl;//Ausgabe deies Fehlers.
    return Nummer;
}

/*!
 * \brief SQLiteQuelle::addKleidungstyp fügt einen neunen Typ von Kleiderstücken hinzu.
 * \param Name Name des hinzu gefügten Typs
 * \param AnfangsNummer Erste Nummer des bereiches aus dem die Nummern vergeben werden sollen
 * \param Endnummer Letzte Nummer aus dem Bereich aus dem die Nummer vergeben werden sollen.
 * \return Nummer des Grade Hinzugefügten Kleidungstyp.
 */
int SQLiteQuelle::addKleidungstyp(QString Name,int AnfangsNummer,int Endnummer)
{
    QSqlQuery Abfrage(QString("INSERT INTO Kleidungstyp ('Name','AnNummer','EndNummer')Values('%1',%2,%3)").arg(Name).arg(AnfangsNummer).arg(Endnummer),Datenbank);
    return Abfrage.lastInsertId().toInt();
}

/*!
 * \brief SQLiteQuelle::addPerson fügt eine Person ein.
 * \param Nachname
 * \param Vorname
 * \param Gruppe
 * \return ID der Hinzugefügten Person
 */
int SQLiteQuelle::addPerson(QString Nachname, QString Vorname,int Gruppe)
{
    QSqlQuery Abfrage("",Datenbank);
    QString SQLString=QString("insert into Personen ('Nachname','Vorname','JF') Values('%1','%2',%3);").arg(Nachname,Vorname).arg(Gruppe);
    Abfrage.exec(SQLString);
    if (Abfrage.lastError().type()!=QSqlError::NoError)
        std::cerr<<Abfrage.lastError().text().toStdString()<<std::endl;
    return Abfrage.lastInsertId().toInt();
}

/*!
 * \brief SQLiteQuelle::createDB erzeugt einen Datenbank in die die Daten geschrieben weerden.
 */
void SQLiteQuelle::createDB()
{
    std::clog<<"Die Tabellen werden in der Datenank erzeugt.\n";
    QSqlQuery Abfrage("",Datenbank);
    Abfrage.exec("create table Kleidungsstuecke(id integer primary key AUTOINCREMENT,Nummer integer,Typ integer,Groesse integer,Traeger integer DEFAULT(0),AnzAusleih integer DEFAULT(0),DatumHin DateTime, Bemerkung varchar, DatumLeihe DateTime)");
    Abfrage.exec("create table Kleidungstyp(id integer primary key AUTOINCREMENT,Name varchar,AnNummer integer,EndNummer integer)");
    Abfrage.exec("create table Personen(id integer primary key AUTOINCREMENT,Nachname varchar,Vorname varchar,Jf integer)");
    Abfrage.exec("create table Jugendfeuerwehr(id integer primary key AUTOINCREMENT,Name varchar)");
    Abfrage.exec("create table Groessen(id integer primary key AUTOINCREMENT, Groesse varchar,Typ integer,Rang integer)");
    Abfrage.exec("INSERT INTO Groessen ('id') VALUES(0)");
    QSqlError Fehler=Abfrage.lastError();
    //std::cerr<<Fehler.text().toStdString();
}

/*!
 * \brief SQLiteQuelle::FehlerAusgabe Beibt den Fehler sowie dien SQL sting zurück bei den der Fehler aufgetreten ist.
 * \param Abfrage
 * \return true Wenn kein Fehler Aufgetreten ist.
 */
bool SQLiteQuelle::FehlerAusgabe(const QSqlQuery& Abfrage)
{
    if (Abfrage.lastError().type()!=QSqlError::NoError)
    {
        std::cerr<<"----------------------------\n\n"<<Abfrage.lastQuery().toStdString()<<"\n---\n "<<Abfrage.lastError().text().toStdString()<<std::endl;
        return false;
    }
    return true;
}

/*!
 * \brief SQLiteQuelle::freieNummer liefert die Erste Freie Nummer für den angegebenen Typ.
 * \param Typ
 * \return Erste freie Bummer
 */
int SQLiteQuelle::freieNummer(int Typ)
{
    QSqlQuery Abfrage("",Datenbank);
    Abfrage.exec(QString("SELECT AnNummer,EndNummer FROM Kleidungstyp WHERE id=%1").arg(Typ));
    int min=0,max=0;
    if(Abfrage.next())
    {
        min=Abfrage.value(0).toInt();//Kleinste Nummer für den Typ
        max=Abfrage.value(1).toInt();//Größte Nummer für den Typ
    }
    else
    {
        return -1;
    }
    Abfrage.clear();
    //Fragt liste aller vergebenen Nummren ab in aufsteigend sortierter Reihenfolge.
    Abfrage.exec(QString("SELECT Nummer FROM Kleidungsstuecke WHERE Typ=%1 ORDER BY Nummer ASC").arg(Typ));
    FehlerAusgabe(Abfrage);
    if (Abfrage.next())
    {
        //Prüft ob die Nummer schon vergeen ist.
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

/*!
 * \brief SQLiteQuelle::getGroessen
 * \param Filter Id für, die die Größen zurückgegeben werden soll.
 * \param anz Größe des Übergebenen Arrays.
 * \return Liste Aller zu den angegebenen Typen
 */
GroessenTabelle *SQLiteQuelle::getGroessen(int *Filter, int anz)
{
    GroessenTabelle *Ausgabe=new GroessenTabelle;
    Ausgabe->Anzahl=0;
    QString SQLString="SELECT Groessen.id, Groesse,name, Rang FROM Groessen,Kleidungstyp WHERE Groessen.Typ=Kleidungstyp.id";
    if (anz>0)
    {
        SQLString=SQLString.append(" AND ( Groessen.Typ= %1").arg(Filter[0]);
        for (int i=1;i<anz;i++)
        {
            SQLString=SQLString.append(" OR Groessen.Typ=%1").arg(Filter[i]);
        }
        SQLString=SQLString.append(" ) ORDER BY Groessen.Rang ASC;");
    }
    //std::cout<<SQLString.toStdString()<<std::endl;
    QSqlQuery Abfrage(SQLString,Datenbank);
    FehlerAusgabe(Abfrage);
    while(Abfrage.next())
    {
        ++Ausgabe->Anzahl;
        Ausgabe->IDs.append(Abfrage.value(0).toInt());
        Ausgabe->Namen.append(Abfrage.value(1).toString());
        Ausgabe->Typ.append(Abfrage.value(2).toString());
        Ausgabe->Rang.append(Abfrage.value(3).toInt());
    }
    return Ausgabe;
}

/*!
 * \brief SQLiteQuelle::getJugendfeuerwehr liefert alle angelegten Jugendfeuerwehren zurück.
 * \return Liste aller jugendfeuerwehren mit ID.
 */
JugendFeuerwehrTabelle *SQLiteQuelle::getJugendfeuerwehr()
{
    JugendFeuerwehrTabelle *Ausgabe=new JugendFeuerwehrTabelle;
    Ausgabe->Anzahl=0;
    QSqlQuery Abfrage("SELECT id,Name FROM Jugendfeuerwehr",Datenbank);
    FehlerAusgabe(Abfrage);
    while(Abfrage.next())
    {
        ++Ausgabe->Anzahl;
        Ausgabe->ID.append(Abfrage.value(0).toInt());
        Ausgabe->Name.append(Abfrage.value(1).toString());
    }
    return Ausgabe;
}

int SQLiteQuelle::getGroessenID(QString Groesse, int TypID)
{
    if (TypID<1)
        return -1;
    QSqlQuery Abfrage(Datenbank);
    Abfrage.prepare("SELECT id FROM Groessen WHERE Typ=:Typ AND Groesse LIKE :Groesse");
    Abfrage.bindValue(0,TypID);
    Abfrage.bindValue(1,Groesse);
    Abfrage.exec();
    FehlerAusgabe(Abfrage);
    int ID =-1;
    if (Abfrage.next())
        ID=Abfrage.value(0).toInt();
    return ID;
}

/*!
 * \brief SQLiteQuelle::getKleider
 * \param Typ Filter für den Typ der Kleidungsstücke
 * \param Groesse Filter für die Größe der Kleidungsstücke
 * \param Traeger Filter für den Träger der Kleidungsstücke
 * \return Lister der Kleiderstücke.
 */
KleiderTabelle *SQLiteQuelle::getKleider(int Typ, int Groesse, int Traeger, QString Nummer, bool Sort)
{
    KleiderTabelle *Ausgabe=new KleiderTabelle;
    Ausgabe->Anzahl=0;
    QString SQLString="SELECT Kleidungsstuecke.id,  Nummer, Groessen.Groesse , Kleidungstyp.Name, Kleidungsstuecke.AnzAusleih, Kleidungsstuecke.DatumHin, Kleidungsstuecke.Bemerkung,Kleidungsstuecke.DatumLeihe FROM Kleidungsstuecke ,Groessen,Kleidungstyp WHERE Kleidungsstuecke.Groesse=Groessen.id AND Kleidungsstuecke.Typ=Kleidungstyp.id";
    if(Typ>0)
        SQLString=SQLString.append(" AND Kleidungsstuecke.Typ=%1").arg(Typ);
    if (Groesse>=0)
        SQLString=SQLString.append(" AND Kleidungsstuecke.Groesse=%1").arg(Groesse); 
    SQLString=SQLString.append(" AND Kleidungsstuecke.Traeger=%1").arg(Traeger);
    if (!Nummer.isEmpty())
        SQLString=SQLString.append(" AND Kleidungsstuecke.Nummer LIKE '%%1%'").arg(Nummer);
    if(Sort)
        SQLString=SQLString.append(" ORDER BY Kleidungsstuecke.Nummer ASC");
    QSqlQuery Abfrage(SQLString,Datenbank);
    FehlerAusgabe(Abfrage);
    while (Abfrage.next())
    {
        ++Ausgabe->Anzahl;
        Ausgabe->ID.append(Abfrage.value(0).toInt());
        Ausgabe->Nummer.append(Abfrage.value(1).toInt());
        QString Groesse=Abfrage.value(2).toString();
        Ausgabe->Groesse.append(Groesse);
        Ausgabe->Typ.append(Abfrage.value(3).toString());
        Ausgabe->AnzahlAusleihen.append(Abfrage.value(4).toInt());
        Ausgabe->Anschaffung.append(Abfrage.value(5).toDateTime());
        Ausgabe->Bemerkung.append(Abfrage.value(6).toString());
        Ausgabe->DatumLeihen.append(Abfrage.value(7).toDateTime());
        if (Groesse.isEmpty())
            Ausgabe->Groesseunbekannt.append(true);
        else
            Ausgabe->Groesseunbekannt.append(false);
    }
    return Ausgabe;
}

/*!
 * \brief SQLiteQuelle::getKleiderinKammer Lieft deine Liste mit allen Kleidungsstücken din der Kleiderkammer.
 * \param Typ Typ der nur angezeigt werden soll.
 * \param Groesse Groeße die nur angezeigt werden soll.
 * \return Liste aller Kleidungsstücke in der Kammer mit den Geforderten eigenschaften.
 */
KleiderTabelle *SQLiteQuelle::getKleiderinKammer(int Typ, int Groesse, QString Nummer,bool Sort)
{
    return getKleider(Typ,Groesse,0,Nummer,Sort);
}

KleiderTabelle *SQLiteQuelle::getKleidervonPerson(int id, int Typ)
{
    return getKleider(Typ,-1,id,QString(),false);
}

bool SQLiteQuelle::getKleidungsInfoByID(int ID, int *Nummer, int *Typ, int *Groesse)
{
    QSqlQuery Abfrage(QString("SELECT Nummer,Typ,Groesse FROM Kleidungsstuecke WHERE id=%1").arg(ID),Datenbank);
    if (Abfrage.next())
    {
        *Nummer=Abfrage.value(0).toInt();
        *Typ=Abfrage.value(1).toInt();
        *Groesse=Abfrage.value(2).toInt();
    }
    return FehlerAusgabe(Abfrage);
}

int SQLiteQuelle::getKleidungsInfoByNummer(int Nummer, QString *Typ, QString *Groesse, QDate *Datum, QString *Traeger,int *TraegerID, QString *Gruppe, QString *Bemerkung,int *Anzahl,QDateTime *LeihDatum)
{
    int ID=-1;
    QSqlQuery Abfrage(QString("SELECT Kleidungsstuecke.id, Kleidungstyp.Name, Kleidungsstuecke.Groesse, Kleidungsstuecke.Traeger, Kleidungsstuecke.AnzAusleih, Kleidungsstuecke.DatumHin, Kleidungsstuecke.Bemerkung, Kleidungsstuecke.DatumLeihe FROM Kleidungsstuecke, Kleidungstyp WHERE Nummer=%1 AND Kleidungstyp.id=Kleidungsstuecke.Typ").arg(Nummer),Datenbank);
    if (Abfrage.next())
    {
        ID=Abfrage.value(0).toInt();
        *Typ=Abfrage.value(1).toString();
        QSqlQuery Info(Datenbank);
        int GroID=Abfrage.value(2).toInt();
        if (GroID==0)
            *Groesse="Unbekannt";
        else
        {
            Info.exec(QString("SELECT Groesse FROM Groessen WHERE id=%1").arg(GroID));
            if (Info.next())
                *Groesse=Info.value(0).toString();
        }
        int TraID=Abfrage.value(3).toInt();
        if (TraID==0)
        {
            *Traeger="";
            *Gruppe="Kleiderkammer";
        }
        else
        {
            Info.exec(QString("SELECT Vorname, Nachname, Name FROM Personen, Jugendfeuerwehr WHERE Jugendfeuerwehr.id=jf AND Personen.id=%1").arg(TraID));
            if (Info.next())
            {
                *Traeger=Info.value(0).toString();
                Traeger->append(" ");
                Traeger->append(Info.value(1).toString());
                *Gruppe=Info.value(2).toString();
                *TraegerID=TraID;

            }
        }
        *Anzahl=Abfrage.value(4).toInt();
        *Datum=Abfrage.value(5).toDate();
        *Bemerkung=Abfrage.value(6).toString();
        *LeihDatum=Abfrage.value(7).toDateTime();
    }
    else
        FehlerAusgabe(Abfrage);
    return ID;
}

Kleidungstypentabelle *SQLiteQuelle::getKleidungstypen()
{
    Kleidungstypentabelle *Ausgabe=new Kleidungstypentabelle;
    QString SQLString="SELECT id, Name,AnNummer,Endnummer FROM Kleidungstyp";
    QSqlQuery Abfrage(SQLString,Datenbank);
    FehlerAusgabe(Abfrage);
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

int SQLiteQuelle::getKleidungsTypID(QString Typ)
{
    QSqlQuery Abfrage(QString("SELECT id FROM Kleidungstyp WHERE Name LIKE '%1'").arg(Typ),Datenbank);
    FehlerAusgabe(Abfrage);
    int ID=-1;
    if (Abfrage.next())
        ID=Abfrage.value(0).toInt();
    return ID;
}

int SQLiteQuelle::getIDByKleidungsNummer(int Nummer)
{
    QSqlQuery Abfrage(QString("SELECT id FROM Kleidungsstuecke WHERE Nummer=%1").arg(Nummer),Datenbank);
    FehlerAusgabe(Abfrage);
    if(!Abfrage.next())
        return -1;
    return Abfrage.value(0).toInt();
}

void SQLiteQuelle::getNummerBereich(int TypID, int *Anfang, int *Ende)
{
    QSqlQuery Abfrage(QString("SELECT AnNummer,EndNummer FROM Kleidungstyp WHERE id=%1").arg(TypID),Datenbank);
    FehlerAusgabe(Abfrage);
    if (Abfrage.next())
    {
        *Anfang=Abfrage.value(0).toInt();
        *Ende=Abfrage.value(1).toInt();
    }
}

bool SQLiteQuelle::KleidungsstueckzuordnenbyID(int ID, int Traeger)
{
    QSqlQuery Abfrage(QString("SELECT Traeger FROM Kleidungsstuecke WHERE id=%1").arg(ID),Datenbank);
    FehlerAusgabe(Abfrage);
    if(Abfrage.next())
    {
        if (Abfrage.value(0).toInt()>0)//Prüft ob das Kleidungsstück schon verlienen ist.
            return false;
        Abfrage.clear();
        Abfrage.prepare("UPDATE Kleidungsstuecke SET 'Traeger'=:Trager,'AnzAusleih'=AnzAusleih+1,'DatumLeihe'=:Date WHERE id=:ID");
        Abfrage.bindValue(0,Traeger);
        Abfrage.bindValue(1,QDateTime::currentDateTime());
        Abfrage.bindValue(2,ID);
        Abfrage.exec();
        FehlerAusgabe(Abfrage);
        return true;
    }
    FehlerAusgabe(Abfrage);
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
    SQLString.append(" ORDER BY Nachname,Vorname ASC");
    //std::cout<< SQLString.toStdString();
    QSqlQuery Abfrage(SQLString,Datenbank);
    FehlerAusgabe(Abfrage);
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

PersonenTabelle *SQLiteQuelle::getPersonen(int *JFFilter, int JFans, QString NamenFilter)
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
    // Zelegen der Eingabe in Einzelteile
    QStringList NamenFilterList=NamenFilter.split(' ');
    if (NamenFilterList.length()>0)
    {
        SQLString.append(" AND (");
        SQLString=SQLString.append(" (Personen.Vorname LIKE '%%1%' OR  Personen.Nachname LIKE '%%1%')").arg(NamenFilterList[0]);
        for (int i=1;i<NamenFilterList.length();++i)
        {
            SQLString=SQLString.append(" AND (Personen.Vorname LIKE '%%1%' OR  Personen.Nachname LIKE '%%1%')").arg(NamenFilterList[i]);
        }
        SQLString.append(")");
    }
    SQLString.append(" ORDER BY Nachname,Vorname ASC");
    //std::cout<< SQLString.toStdString();
    QSqlQuery Abfrage(SQLString,Datenbank);
    FehlerAusgabe(Abfrage);
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

bool SQLiteQuelle::getPersonenInfo(int ID, QString *VorName, QString *Nachnanme, QString *Gruppe, int *GruppenID)
{
    QSqlQuery Abfrage(QString("SELECT Personen.Vorname,Personen.Nachname,Jugendfeuerwehr.Name,Personen.Jf FROM Personen,Jugendfeuerwehr WHERE Personen.id=%1 AND Personen.Jf=Jugendfeuerwehr.id").arg(ID),Datenbank);
    if (Abfrage.next())
    {
        *VorName=Abfrage.value(0).toString();
        *Nachnanme=Abfrage.value(1).toString();
        *Gruppe=Abfrage.value(2).toString();
        *GruppenID=Abfrage.value(3).toInt();
        return true;
    }
    return false;
}

bool SQLiteQuelle::removeGrosse(int ID)
{
    QSqlQuery Abfrage(QString("SELECT ID FROM Kleidungsstuecke WHERE Groesse=%1").arg(ID),Datenbank);
    if (!FehlerAusgabe(Abfrage))
            return false;
    if (Abfrage.next())
        return false;
    Abfrage.exec(QString("DELETE FROM Groessen WHERE id=%1").arg(ID));
    return FehlerAusgabe(Abfrage);
}

bool SQLiteQuelle::removeJugendferweher(int ID)
{
    QSqlQuery Abfrage(QString("DELETE FROM Jugendfeuerwehr WHERE id=%1").arg(ID),Datenbank);
    return FehlerAusgabe(Abfrage);
}

bool SQLiteQuelle::removeKleidungsstueck(int ID)
{
    QSqlQuery Abfrage(QString("DELETE FROM Kleidungsstuecke WHERE id=%1").arg(ID),Datenbank);
    return FehlerAusgabe(Abfrage);
}

bool SQLiteQuelle::removeKleidungstyp(int ID)
{
    QSqlQuery Abfrage(QString("SELECT id FROM Kleidungsstuecke WHERE Typ=%1").arg(ID),Datenbank);
    if (!FehlerAusgabe(Abfrage))
            return false;
    if (Abfrage.next())
        return false;
    Abfrage.exec(QString("DELETE FROM Kleidungstyp WHERE id=%1").arg(ID));
    return FehlerAusgabe(Abfrage);
}

bool SQLiteQuelle::removePerson(int ID)
{
    QSqlQuery Abfrage(QString("DELETE FROM Personen WHERE id=%1").arg(ID),Datenbank);
    return FehlerAusgabe(Abfrage);
}

void SQLiteQuelle::rueckgabeKleidungsstueck(int ID)
{
    QSqlQuery Abfrage(Datenbank);
    Abfrage.prepare(QString("UPDATE Kleidungsstuecke SET 'Traeger'=0 , 'DatumLeihe'=:Date WHERE id=:ID"));
    Abfrage.bindValue(0,QDateTime::currentDateTime());
    Abfrage.bindValue(1,ID);
    Abfrage.exec();
    FehlerAusgabe(Abfrage);
}

bool SQLiteQuelle::setKleidungsGroesse(int ID, int GroesseID)
{
    QSqlQuery Abfrage(QString("SELECT Groesse FROM Kleidungsstuecke WHERE id=%1").arg(ID),Datenbank);
    if (!Abfrage.next())
        return false;
    std::clog<<Abfrage.value(0).toString().toStdString();
    if (Abfrage.value(0).toString().compare("0")==0)
    {
        Abfrage.exec(QString("UPDATE Kleidungsstuecke SET 'Groesse'=%2 WHERE id=%1").arg(ID).arg(GroesseID));
        return FehlerAusgabe(Abfrage);
    }
    return false;
}

bool SQLiteQuelle::setKleidungsKommentar(int ID, QString Kommentar)
{
    QSqlQuery Abfrage(Datenbank);
    Abfrage.prepare("UPDATE Kleidungsstuecke SET 'Bemerkung'=:Kommentar WHERE id=:ID");
    Abfrage.bindValue(0, Kommentar);//Einsetzen der Daten
    Abfrage.bindValue(1, ID);
    Abfrage.exec();
    return true;
}

bool SQLiteQuelle::setRangGroesse(int ID, int Rang)
{
    QSqlQuery Abfrage(Datenbank);
    Abfrage.prepare("UPDATE Groessen SET 'Rang'=:Kommentar WHERE id=:ID");
    Abfrage.bindValue(0, Rang);//Einsetzen der Daten
    Abfrage.bindValue(1, ID);
    Abfrage.exec();
    return true;;
}

/*!
 * \brief SQLiteQuelle::setPersonNachname änder den Nachnamen der Person auf den Angegeben Nachnamen.
 * \param ID ID der Person von der der Name geändert werden soll.
 * \param Vorname Der neue Nachname der Person
 * \return ob das Ändern Erfolgereich war.
 */
bool SQLiteQuelle::setPersonNachname(int ID, QString Nachname)
{
    QSqlQuery Abfrage(QString("UPDATE Personen SET 'Nachname'='%2' WHERE id=%1").arg(ID).arg(Nachname),Datenbank);
    return FehlerAusgabe(Abfrage);
}

/*!
 * \brief SQLiteQuelle::setPersonToGruppe ändert die Gruppe in der sich eine Person befindet.
 * \param Person ID der Person von der die Gruppe geändert wird.
 * \param Gruppe Id Der neuen Gruppe.
 * \return ob das setzen erfolgreich war.
 */
bool SQLiteQuelle::setPersonToGruppe(int Person, int Gruppe)
{
    QSqlQuery Abfrage(QString("UPDATE Personen SET 'Jf'=%2 WHERE id=%1").arg(Person).arg(Gruppe),Datenbank);
    return FehlerAusgabe(Abfrage);
}

/*!
 * \brief SQLiteQuelle::SetPersonVorname änder den Vornamen der Person auf den Angegeben Vornamen.
 * \param ID ID der Person von der der Name geändert werden soll.
 * \param Vorname Der neue Vorname der Person
 * \return ob das Ändern Erfolgereich war.
 */
bool SQLiteQuelle::setPersonVorname(int ID, QString Vorname)
{
    QSqlQuery Abfrage(QString("UPDATE Personen SET 'Vorname'='%2' WHERE id=%1").arg(ID).arg(Vorname),Datenbank);
    return FehlerAusgabe(Abfrage);
}
