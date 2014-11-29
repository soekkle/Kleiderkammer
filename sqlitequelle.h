#ifndef SQLITEQUELLE_H
#define SQLITEQUELLE_H

#include "datenquelle.h"
#include <iostream>
#include <QtSql>

//! Klasse die den Zugriff auf die Datenbank darstellt.
/*!
 * \brief Die Klasse SQLiteQuelle stellt die Daten aus einer SQLite Datenbank zur verfügung. Und implementirt die
 *Funktionen der Klasse Datenquelle.
 *version :0.1
 *author :soekkle
 */

class SQLiteQuelle : public DatenQuelle
{
public:
    SQLiteQuelle(QString Pfad);
    //! Fügt eine Größe hinzu.
    int addGroesse(QString Groesse,int Typ);
    //! Fügt einen JugendFeuerweher hinzu.
    int addJugendfeuerwehr(QString Name);
    //! Fügt ein Kleidngsstück den Datenbestand hinzu.
    int addKleiderstueck(int Typ, int Groesse, int Nummer=-1);
    //! Fügt einen Kleidungstyp hinzu.
    int addKleidungstyp(QString Name, int AnfangsNummer, int Endnummer);
    //! Fügt einen Person den Datenbestand hinzu.
    int addPerson(QString Nachname, QString Vorname,int Gruppe);
    //! Liefert die Erste Freie Nummer für den Typ
    int freieNummer(int Typ);

    GroessenTabelle *getGroessen(int *Filter, int anz);
    Kleidungstypentabelle *getKleidungstypen();
    JugendFeuerwehrTabelle *getJugendfeuerwehr();
    PersonenTabelle *getPersonen(int *JFFilter,int JFans);

    int getIDByKleidungsNummer(int Nummer);
    //! Fügt einen Kleidungstück einen Träger hinzu.
    bool KleidungsstueckzuordnenbyID(int ID, int Traeger);

    bool removeGrosse(int ID);
    bool removeJugendferweher(int ID);
    bool removeKleidungstyp(int ID);
    bool removePerson(int ID);
private:
    QSqlDatabase Datenbank;
    //! Erstellt die Datenbank.
    void createDB();
};

#endif // SQLITEQUELLE_H
