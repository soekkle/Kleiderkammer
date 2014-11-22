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
    //! Fügt einen Person den Datenbestand hinzu.
    int addPerson(QString Nachname, QString Vorname,int Gruppe);
    //! Fügt ein Kleidngsstück den Datenbestand hinzu.
    int addKleiderstueck(int Typ, QString Groesse, int Nummer=-1);
    int freieNummer(int Typ);
private:
    QSqlDatabase Datenbank;
    //! Erstellt die Datenbank.
    void createDB();
};

#endif // SQLITEQUELLE_H
