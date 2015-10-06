/*
 * Copyright (C) 2014,2015 Sören Krecker
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
 * Diese Datei ist Teil von Kleiderkammer.
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
    //! Liefert eine Liste mit den Größen die die Filtereigenschaften der Typen erfülen
    GroessenTabelle *getGroessen(int *Filter, int anz);
    //! Liefert die ID zu einer Größe von Einen Bestimten Kleidungstyp
    int getGroessenID(QString Groesse,int TypID);
    KleiderTabelle *getKleiderinKammer(int Typ, int Groesse);
    KleiderTabelle *getKleidervonPerson(int id, int Typ);
    int getKleidungsInfoByNummer(int Nummer, QString *Typ, QString *Groesse, QDate *Datum, QString *Traeger,int *TraegerID, QString *Gruppe, QString *Bemerkung,int *Anzahl);
    Kleidungstypentabelle *getKleidungstypen();
    //! Liefert die ID eines Kleidungstyps.
    int getKleidungsTypID(QString Typ);
    JugendFeuerwehrTabelle *getJugendfeuerwehr();
    PersonenTabelle *getPersonen(int *JFFilter,int JFans);
    PersonenTabelle *getPersonen(int *JFFilter, int JFans, QString NamenFilter);
    bool getPersonenInfo(int ID, QString *VorName,QString *Nachnanme,QString *Gruppe, int * GruppenID);
    void getNummerBereich(int TypID,int *Anfang,int* Ende);
    int getIDByKleidungsNummer(int Nummer);
    //! Fügt einen Kleidungstück einen Träger hinzu.
    bool KleidungsstueckzuordnenbyID(int ID, int Traeger);

    bool removeGrosse(int ID);
    bool removeJugendferweher(int ID);
    bool removeKleidungstyp(int ID);
    bool removePerson(int ID);
    //! Löscht das Kleidungsstück aus der Datenbank.
    bool removeKleidungsstueck(int ID);
    void rueckgabeKleidungsstueck(int ID);
    //! Setzt die Größe eines Kleidungsstück wenn sie Unbekannt ist.
    bool setKleidungsGroesse(int ID, int GroesseID);
    //! Setzt den Komentar eines Kleidungsstückes.
    bool setKleidungsKommentar(int ID, QString Kommentar);
    //! Setzt den Rang der Größe zum Sortiern in den Listen.
    bool setRangGroesse(int ID,int Rang);
private:
    QSqlDatabase Datenbank;
    //! Erstellt die Datenbank.
    void createDB();
    bool FehlerAusgabe(QSqlQuery Abfrage);
    KleiderTabelle* getKleider(int Typ, int Groesse,int Traeger);
};

#endif // SQLITEQUELLE_H
