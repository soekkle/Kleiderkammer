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
    KleiderTabelle *getKleiderinKammer(int Typ, int Groesse);
    KleiderTabelle *getKleidervonPerson(int id, int Typ);
    Kleidungstypentabelle *getKleidungstypen();
    JugendFeuerwehrTabelle *getJugendfeuerwehr();
    PersonenTabelle *getPersonen(int *JFFilter,int JFans);

    void getNummerBereich(int TypID,int *Anfang,int* Ende);
    int getIDByKleidungsNummer(int Nummer);
    //! Fügt einen Kleidungstück einen Träger hinzu.
    bool KleidungsstueckzuordnenbyID(int ID, int Traeger);

    bool removeGrosse(int ID);
    bool removeJugendferweher(int ID);
    bool removeKleidungstyp(int ID);
    bool removePerson(int ID);
    void rueckgabeKleidungsstueck(int ID);
    //! Setzt den Komentar eines Kleidungsstückes.
    bool setKleidungsKommentar(int ID, QString Kommentar);
    bool setRangGroesse(int ID,int Rang);
private:
    QSqlDatabase Datenbank;
    //! Erstellt die Datenbank.
    void createDB();
    bool FehlerAusgabe(QSqlQuery Abfrage);
    KleiderTabelle* getKleider(int Typ, int Groesse,int Traeger);
};

#endif // SQLITEQUELLE_H
