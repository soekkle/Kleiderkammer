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

#ifndef DATENQUELLE_H
#define DATENQUELLE_H

#include <QString>
#include <QVector>
#include <QDateTime>

//! Hilfsklasse zur Ausgabe der Größen.
class GroessenTabelle
{
public:
    //! Anzahlder Größen in dem objekt
    int Anzahl;
    //! IDs der Größen
    QVector<int> IDs;
    //! Namen der Größen
    QVector<QString> Namen;
    //! Typen zun den die Größe Gehört
    QVector<QString> Typ;
    //! Rang nach dem die Größen sortiert werden
    QVector<int> Rang;
};

//! Hilfsklasse zur Ausgabe der Jugendfeuerwehren.
class JugendFeuerwehrTabelle
{
public:
    int Anzahl;
    QVector<int>ID;
    QVector<QString>Name;
};

//! Klasse zum zwischenspeichern von Abfrageergebnissen mit Kleidungsstücken.
class KleiderTabelle
{
public:
    //! Anzahl der Kleidungsstücke
    int Anzahl;
    //! ID des Kleidungsstück
    QVector<int> ID;
    //! Numme des Kleiudungsstücks
    QVector<int> Nummer;
    //! Typ des Kleidungsstück
    QVector<QString> Typ;
    //! Größe des Kleidungsstücks
    QVector<QString> Groesse;
    QVector<int> AnzahlAusleihen;
    QVector<QDateTime> Anschaffung;
    QVector<QString> Bemerkung;
    QVector<bool> Groesseunbekannt;
};

class Kleidungstypentabelle
{
public:
    int Anzahl;
    QVector<QString> Name;
    QVector<int> ID;
    QVector<int> AnfangsNummer;
    QVector<int> EndNummer;
};

//! Klassre zur Speicherung von Rückgabedaten über Personen
class PersonenTabelle
{
public:
    int Anzahl;
    QVector<int> ID;
    QVector<QString> Vorname;
    QVector<QString> Nachname;
    QVector<QString> JugendFeuerwehr;
};
//! Klasse die die Schnittstelle zu eine DatenQuelle Definirt.
/*!
 * \brief Die Klasse DatenQuelle stellt eine Schnittstelle zum Abfragen und setzen der Daten in der Kleiderkammer zur
 * Verfügung.
 */
class DatenQuelle
{
public:
    DatenQuelle();
    virtual ~DatenQuelle();
    //! Fügt eine Größe hinzu.
    /*!
     * \brief addGroesse Fügt eine Größe in die DatenQuelle hinzu mit den übegebenen eigenschften.
     * \param Groesse Name der Größe
     * \param Typ ID des Kleidungsstyps zu dem die Größe gehört
     * \return
     */
    virtual int addGroesse(QString Groesse,int Typ)=0;
    //! Fügt einen JugendFeuerweher hinzu.
    virtual int addJugendfeuerwehr(QString Name)=0;
    //! Fügt ein Kleidungsstück hinzu.
    /*!
     * \brief addKleiderstueck fügt ein Kleidungsstück in die DatenQuelle hinzu. Es weden die Paramter des
     * Kleidungsstückes benötigt.
     * \param Typ ID des Typs des Kleidungsstückes
     * \param Groesse ID der Größe des Kleidungsstückes
     * \param Nummer Gewünschte Numme des Kleidungsstückes Optimal
     * \return Nummer des Kleidungsstückes
     */
    virtual int addKleiderstueck(int Typ,int Groesse,int Nummer=-1)=0;
    //! Fügt einen Kleidungstyp hinzu
    /*!
     * \brief addKleidungstyp fügt in die DatenQuelle einen neuen Typ von Kleidungsstücken hinzu.
     * \param Name Name des Typs.
     * \param AnfangsNummer Erste Nummer die Vegeben werden soll.
     * \param Endnummer Letzte Nummer die Vergeben weden soll.
     * \return
     */
    virtual int addKleidungstyp(QString Name, int AnfangsNummer, int Endnummer)=0;
    //! Fügt der Datenquelle eine Person hinzu
    virtual int addPerson(QString Nachname, QString Vorname,int Gruppe)=0;
    virtual int freieNummer(int Typ)=0;
    //! Liefert eine Liste mit den Größen die die Filtereigenschaften der Typen erfülen
    virtual GroessenTabelle *getGroessen(int *Filter, int anz)=0;
    //! Liefert die ID zu einer Größe von Einen Bestimten Kleidungstyp
    virtual int getGroessenID(QString Groesse,int TypID)=0;
    virtual int getIDByKleidungsNummer(int Nummer)=0;
    virtual JugendFeuerwehrTabelle *getJugendfeuerwehr()=0;
    virtual KleiderTabelle *getKleidervonPerson(int id,int Typ)=0;
    virtual KleiderTabelle *getKleiderinKammer(int Typ,int Groesse)=0;
    //! Liefert Informationen zu dem Kleidungsstück mit der ID.
    /*!
     * \brief getKleidungsInfoByID liefert einige Informationen zu dem Kleidungsstück mit der übergebenen ID
     * \param ID[in] ID des Kleidungsstückes
     * \param Nummer[out] Nummer unter der das Kleidungsstück geführt wird
     * \param Typ[out] Id des Kleidungsstyps des Kleidungsstckes
     * \param Groesse[out] ID der Kleidungsgröße des Kleidungsstückes
     * \return Ob das Kleidungsstück existiert.
     */
    virtual bool getKleidungsInfoByID(int ID,int *Nummer, int *Typ, int *Groesse)=0;
    //! Liefert die Informationen zu einen Kleidungsstück
    /*!
     * \brief  getKleidungsInfoByNummer liefert alle gespeicherten Informationen zu einen Kleidungsstück mit der
     * Angegeben Nummer. Wenn zwei Kleidungsstücke mit der selben Nummer vorhanden sind, ist das Verhalten nicht
     * definirt
     * \param[in] Nummer Nummer des Kleidungsstücks
     * \param[out] Typ Typ des gesuchten Kleidungsstücks
     * \param[out] Groesse Größe des gesuchten Kleidungsstücks
     * \param[out] Datum Datum an den das Kleidungsstück angelegt wurde
     * \param[out] Traeger Name des aktuellen Trägers
     * \param[out] TraegerID ID des aktuellen Trägers
     * \param[out] Gruppe Gruppe des aktuellen Trägers
     * \param[out] Bemerkung Bemerkung zu dem Kleidungsstück
     * \param[out] Anzahl Wieoft das Kleidungsstück Ausgeliehnen wurde
     * \return ID Des Ausgegbenen Kleidungsstücks
     */
    virtual int getKleidungsInfoByNummer(int Nummer, QString *Typ, QString *Groesse, QDate *Datum, QString *Traeger,int *TraegerID, QString *Gruppe, QString *Bemerkung,int *Anzahl)=0;
    virtual Kleidungstypentabelle *getKleidungstypen()=0;
    //! Liefert die ID eines Kleidungstyps.
    virtual int getKleidungsTypID(QString Typ)=0;
    virtual void getNummerBereich(int TypID,int *Anfang,int* Ende)=0;
    //! Liefet die Liste mit den Namen und ID der Personen die den Filter erfüllen.
    /*!
     * \brief getPersonen liefet einen Liste mit Namen und weiteren Information zu den Pesonen. Die in den per Filte
     * angegbenen Guppen sich befinden.
     * \param JFFilter int Array mit den IDs der Gruppen.
     * \param JFans Größe des Arrays
     * \return List der Personen
     */
    virtual PersonenTabelle *getPersonen(int *JFFilter,int JFans)=0;
    //! Liefet die Liste mit den Namen und ID der Personen die den Filter erfüllen.
    /*!
     * \brief getPersonen liefet einen Liste mit Namen und weiteren Information zu den Pesonen. Die in den per Filte
     * angegbenen Guppen sich befinden und Teile des Namen Enthalten.
     * \param JFFilter int Array mit den IDs der Gruppen.
     * \param JFans Größe des Arrays
     * \param NamenFilte Sting der im Vor oder Nachnamen enthalten ist.
     * \return List der Personen
     */
    virtual PersonenTabelle *getPersonen(int *JFFilter, int JFans,QString NamenFilter)=0;
    //! Liefert Informationen zu einer Person
    /*!
     * \brief getPersonenInfo liefert Name usw. zu einer übergebenen ID aus der DatenQuelle.
     * \param ID ID nach der Gesucht wird
     * \param VorName Vorname der Person
     * \param Nachnanme Nachname der Person
     * \param Gruppe Name der Gruppe
     * \param GruppenID Id der Gruppe
     * \return Ob die Abfrage erfolgreich war.
     */
    virtual bool getPersonenInfo(int ID, QString *VorName,QString *Nachnanme,QString *Gruppe, int * GruppenID)=0;
    virtual bool KleidungsstueckzuordnenbyID(int ID,int Traeger)=0;
    virtual bool removeGrosse(int ID)=0;
    virtual bool removeJugendferweher(int ID)=0;
    virtual bool removeKleidungsstueck(int ID)=0;
    virtual bool removeKleidungstyp(int ID)=0;
    virtual bool removePerson(int ID)=0;
    virtual void rueckgabeKleidungsstueck(int ID)=0;
    virtual bool setKleidungsGroesse(int ID, int GroesseID)=0;
    virtual bool setKleidungsKommentar(int ID, QString Kommentar)=0;
    //! Setzt den Rang einer Größe
    /*!
     * \brief setRangGroesse setzt den Rang zum Sörtieren einer Größe.
     * \param ID Id der Größe von der der Rang gesezt werden soll.
     * \param Rang Wert auf den der rang gesetzt wird.
     * \return Status des setzen.
     */
    virtual bool setRangGroesse(int ID,int Rang)=0;

};

#endif // DATENQUELLE_H
