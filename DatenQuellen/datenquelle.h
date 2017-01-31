/*
 * Copyright (C) 2014-2016 Sören Krecker
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
    //! Datum des letzten Ausleihens oder zurückgebens.
    QVector<QDateTime> DatumLeihen;
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

//! Klasse zur Speicherung von Rückgabedaten über Personen
/*!
 * \brief Die Klasse PersonenTabelle speichert daten zu verschieden Personen als structure of arryas
 */
class PersonenTabelle
{
public:
    //! Anzahl der Personen in dem Objekt
    int Anzahl;
    //! ID der Dersonen
    QVector<int> ID;
    //! Vornamen der Personen
    QVector<QString> Vorname;
    //! Nachnanmen der Personen
    QVector<QString> Nachname;
    //! Gruppe der Personen
    QVector<QString> JugendFeuerwehr;
};

//! Klasse die die Schnittstelle zu eine DatenQuelle Definirt.
/*!
 * \brief Die Klasse DatenQuelle stellt eine abstrakte Schnittstelle zum Abfragen und setzen der Daten in der
 *  Kleiderkammer zur Verfügung.
 */
class DatenQuelle
{
public:
    DatenQuelle();
    virtual ~DatenQuelle();
    //! Fügt eine Größe hinzu.
    /*!
     * \brief addGroesse Fügt eine Größe in die DatenQuelle hinzu mit den übegebenen Eigenschften.
     * \param Groesse Name der Größe
     * \param Typ ID des Kleidungsstyps zu dem die Größe gehört
     * \return
     */
    virtual int addGroesse(QString Groesse,int Typ)=0;
    //! Fügt einen JugendFeuerweher hinzu.
    /*!
     * \brief addJugendfeuerwehr fügt eine neue Gruppe für die Personen hinzu.
     * \param Name Name der neuen Gruppe
     * \return ID der Gruppe
     */
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
    virtual int addKleiderstueck(int Typ,int Groesse,int Nummer=-1,bool uberschreibe=false)=0;
    //! Fügt einen Kleidungstyp hinzu.
    /*!
     * \brief addKleidungstyp fügt in die DatenQuelle einen neuen Typ von Kleidungsstücken hinzu.
     * \param Name Name des Typs.
     * \param AnfangsNummer Erste Nummer die Vegeben werden soll.
     * \param Endnummer Letzte Nummer die Vergeben weden soll.
     * \return
     */
    virtual int addKleidungstyp(QString Name, int AnfangsNummer, int Endnummer)=0;
    //! Fügt der Datenquelle eine Person hinzu.
    /*!
     * \brief addPerson fügt eine neue Person in den Bestand hinzu. Die Berson hat die Übergeben eigenschaften und
     * noch keine Kleidung. Wenn ein Fehler Auftritt wird -1 als ID zurückgegeben.
     * \param Nachname Nachname der neuen Person
     * \param Vorname Vorname der neuen Person
     * \param Gruppe Gruppe zu der die neue Person gehört
     * \return ID der hinzugefügten Person
     */
    virtual int addPerson(QString Nachname, QString Vorname,int Gruppe)=0;
    //! Liefert die erste Freie Nummer des Typs zurück.
    /*!
     * \brief freieNummer liefert die erste freie Nummer für ein Kleidungsstück des übergebenen Typs aus dem hinterlegten
     * Nummerbereich des Kleidungsstyps.
     * \param Typ ID des Typs.
     * \return Erste freie Nummer
     */
    virtual int freieNummer(int Typ)=0;
    //! Liefert eine Liste mit den Größen die die Filtereigenschaften der Typen erfülen.
    /*!
     * \brief getGroessen Liefert eine Liste mit den Größen für die als Filter übergeben Kleidungstypen. Die
     * Kleidungstypen werden durch ihre ID übergeben.
     * \param Filter Int Arrya mit den Filter ID
     * \param anz Anzahl der Filter
     * \return Tabelle mit den entsprechenden Größen.
     */
    virtual GroessenTabelle *getGroessen(int *Filter, int anz)=0;
    //! Liefert die ID zu einer Größe von Einen Bestimten Kleidungstyp.
    /*!
     * \brief getGroessenID gibt eine ID zurück die zu einer Größe Passt, die bis auf Groß- und Kleinschreibeung der
     * Übergebenen Größe entspricht. Wenn mehrere Größen mit der Bezeichnung für den Typ vorhanden sind, ist das
     * Verhalten nicht definirt.
     * \param Groesse Name der Größe
     * \param TypID ID des Kleidertyps der Größe
     * \return
     */
    virtual int getGroessenID(QString Groesse,int TypID)=0;
    //! Liefert die ID zu einen Kleidungsstük mit der übergeben Nummer.
    /*!
     * \brief getIDByKleidungsNummer liefert die ID des Kleidungstückes mit der übergeben Nummer. Wenn die Nummer
     * doppeltvergeben ist, ist das verhalten nicht definirt.
     * \param Nummer Nummer des Kleidungsstückes
     * \return ID des Kleidungsstückes
     */
    virtual int getIDByKleidungsNummer(int Nummer)=0;
    //! Liefert einen Pointer auf ein Struckture of Array mit den daten aller Angelegten Gruppen.
    /*!
     * \brief getJugendfeuerwehr liefert eine Liste mit allen angelegten Gruppen zurück. Sie hat keine Eingabeparameter.
     *
     * \return Zeiger auf die  Struckture of Array mit den Daten.
     */
    virtual JugendFeuerwehrTabelle *getJugendfeuerwehr()=0;
    //! Gibt einen Liste mit den Kleidungsstücken des typs der Person zurück.
    /*!
     * \brief getKleidervonPerson liefert eine Liste mit den Typentsprechenden Kleidungstyp der angegeben Person zurück.
     * \param id ID der Person von der die Kleidungsstücke ausgegebnen werden soll.
     * \param Typ Kleidungsstyp nach dem Gefilter wird.
     * \return TAbelle mit den Kleidungsstcken einer Person.
     */
    virtual KleiderTabelle *getKleidervonPerson(int id,int Typ)=0;
    //! Gibt eine Liste mit den Kleidungsstückenn die in der Kleiderkammer sind und der Größe entsprechen.
    /*!
     * \brief getKleiderinKammer liefer einen Pointer auf ein Structure of Arrys, mit den Kleidungsstücken die in der
     * Kleiderkammer sind und den übergebenen Filtern entsprechen, zurück.
     * \param Typ Kleidungstyp nach dem gefilter wird.
     * \param Groesse Größe nach der Gefilter wird.
     * \return Pointer auf die Liste mit den Kleidungsstücken.
     */
    virtual KleiderTabelle *getKleiderinKammer(int Typ, int Groesse, QString Nummer=QString())=0;
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
    virtual int getKleidungsInfoByNummer(int Nummer, QString *Typ, QString *Groesse, QDate *Datum, QString *Traeger,int *TraegerID, QString *Gruppe, QString *Bemerkung,int *Anzahl,QDateTime *LeihDatum)=0;
    //! Liefert alle Kleidungstypen zurück.
    /*!
     * \brief getKleidungstypen liefert einen Pointer auf ein Strucktur of Array mit informationen zu allen Größen
     *  wieder.
     * \return Pointer der Liste.
     */
    virtual Kleidungstypentabelle *getKleidungstypen()=0;
    //! Liefert die ID eines Kleidungstyps.
    /*!
     * \brief getKleidungsTypID gibt die ID eines Kleidungstyps zurück der mit den übergeben Namen übereinstimmt.
     * Bei doppeldeutigkeit ist das verhalten der Funktion nicht definirt und von der konkreten klasse abhänig. Beim
     * vergleich der Namen wird nicht auf Groß- und Kleinschreibung geachtet.
     * \param Typ Name des Kleidungstyps
     * \return ID des Kleidungstyps.
     */
    virtual int getKleidungsTypID(QString Typ)=0;
    //! Liefert den Bereich der Nummern der für einen Kleidungstypen vorgesehen ist.
    /*!
     * \brief getNummerBereich setzt dei Variabelen Anfang und dende auf die Werte des Bereiches der Nummern die
     * für einen Kleidungstypen vorgesehen sind. Die Pointer Anfang und Ende müssen auf existierende Variablen zeigen.
     * \param [in]TypID ID des abzufragenden Kleidungspypen
     * \param [out]Anfang Zeiger auf die Erste Nummer
     * \param [out]Ende Zeiger auf die Letzte Nummer
     */
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
    //! Ordnet einen Trägeren einen Kleidungsstück zu.
    /*!
     * \brief KleidungsstueckzuordnenbyID stellt die Verbindung zwieschen einen Kleidungsstück und einer Person die
     * das Kleidungsstück hat her. Eine Perosn kann mehrere Kleidungsstücke haben, aber ein Kleidungstück kann nur
     * einer Person zugeordnet werden.
     * \param ID ID des Kleidungsstückes
     * \param Traeger ID der Person
     * \return Ob das zuordnen erfolgreich war
     */
    virtual bool KleidungsstueckzuordnenbyID(int ID,int Traeger)=0;
    //! Entfernt eine Größe aus der DatenQuelle.
    /*!
     * \brief removeGrosse entfernt die Größe mit der angegebenen ID aus dem Speicher der DatenQuelle.
     * Wenn noch Kleidungsstücke mit der Größe vorhanden sind soll das entfernen fehlschlagen.
     * \param ID ID der Größe die Entfend werden soll.
     * \return Ob das Entfernen erfolgreich war
     */
    virtual bool removeGrosse(int ID)=0;
    //! Entfernd eine Gruppe aus der Datenquelle
    /*!
     * \brief removeJugendferweher entfernd einen Gruppe mit der angegeben ID aus der Datenquelle. Die Gruppe soll
     * sich nur entfernen lassen, wenn keine Person mehr in der Gruppe ist.
     * \param ID ID der zu löschenden Gruppe
     * \return Ob das Löschen erfolgreich war
     */
    virtual bool removeJugendferweher(int ID)=0;
    //! Entfernd ein Kleidungsstück aus der DatenQuelle.
    /*!
     * \brief removeKleidungsstueck löscht das Kleidungsstücj mit der Angegebenen ID.
     * Wenn das Kleidungsstück vergeben ist, hängt das verhalten von der Implementierung der Funktion ab.
     * \param ID ID des Kleidungsstücks
     * \return Ob das Löschen erfolgreich war
     */
    virtual bool removeKleidungsstueck(int ID)=0;
    //! Entfernd einen Kleidungstyp aus der DatenQuelle.
    /*!
     * \brief removeKleidungstyp entfernd den Kleidungstyp mit der angegeben ID aus der DatenQuelle. Das entfernen ist
     *  nur möglich wenn kein Kleidungsstück von diesem Kleidungsstyp existiert.
     * \param ID ID des KleidungsTyp
     * \return Ob das Entfernen erfolgreich war
     */
    virtual bool removeKleidungstyp(int ID)=0;
    //! Entfernt eine Person aus der Datenquelle.
    /*!
     * \brief removePerson entfernt die Person mit der angebenen ID aus der DatenQuelle, wenn die Peron keine
     * Kleidungsstücke mehr hat. Sonst ist das entfernen der Person nicht möglich. Die Funktion gibt zurück ob das
     * Entfernen der Person efrolgreich ist.
     * \param ID ID der Peron die
     * \return Ob das Entfernen erfolgreich war
     */
    virtual bool removePerson(int ID)=0;
    //! Markiert ein Kleidungsstück als zurückgegeben.
    /*!
     * \brief rueckgabeKleidungsstueck markiert das Kleidungsstück mit der angegeben ID als frei und das es in der
     * Kleiderkammer liegt.
     * \param ID ID Des zurückgegeben Kleidungsstücks
     */
    virtual void rueckgabeKleidungsstueck(int ID)=0;
    //! Setzt nachträglich die Größe eines Kleidungsstückes.
    /*!
     * \brief setKleidungsGroesse setzt die Größe des angegbenen Kleidungsstückes auf die angegebene Größe.
     * Diese Klappt nur einmal bei Kleidungsstücke mit der Größe "Größe Unbekannt". Ist das Setzen nicht erfolgreich
     * das ein Fehler auftritt oder das Kleidungsstück schon eine Größe hat, so wird false zurückgegeben.
     * \param ID ID des Kleidungsstückes
     * \param GroesseID ID der Größe des Kleidungsstückes
     * \return Ob das setzen der Größe erfolgreich war.
     */
    virtual bool setKleidungsGroesse(int ID, int GroesseID)=0;
    //! Setzt den Kommentar eines Kleidungsstückes.
    /*!
     * \brief setKleidungsKommentar Überschreibt den Inhalt des Kommentarfeldes eines Kleidungsstückes.
     * \param ID ID des Kleidungsstückes
     * \param Kommentar Inhalt des Kommentars
     * \return false wenn ein Fehler aufgetreten ist
     */
    virtual bool setKleidungsKommentar(int ID, QString Kommentar)=0;
    //! Setzt den Rang einer Größe
    /*!
     * \brief setRangGroesse setzt den Rang zum Sortieren einer Größe.
     * \param ID Id der Größe von der der Rang gesezt werden soll.
     * \param Rang Wert auf den der rang gesetzt wird.
     * \return Status des setzen.
     */
    virtual bool setRangGroesse(int ID,int Rang)=0;
    //! Andert den Nachnamen der Person
    /*!
     * \brief SetPersonNachname änder den Nachnamen der Person auf den Angegeben Nachnamen.
     * \param ID ID der Person von der der Name geändert werden soll.
     * \param Vorname Der neue Nachname der Person
     * \return ob das Ändern Erfolgereich war.
     */
    virtual bool setPersonNachname(int ID, QString Nachname)=0;
    //! Ändert die Gruppe zu der Eine Person Gehört
    /*!
     * \brief setPersonToGruppe Fügt die Person zu einer neuen Gruppe hinzu und entfrend sie aus der alten Gruppe
     * \param PersonenID ID der Person die die Gruppe wechsel.
     * \param GruppenID ID Der neuen Gruppe.
     * \return ob das wechsel erfolgreich war.
     */
    virtual bool setPersonToGruppe(int PersonenID,int GruppenID)=0;
    //! Andert den Vornamen der Person
    /*!
     * \brief SetPersonVorname änder den Vornamen der Person auf den Angegeben Vornamen.
     * \param ID ID der Person von der der Name geändert werden soll.
     * \param Vorname Der neue Vorname der Person
     * \return ob das Ändern Erfolgereich war.
     */
    virtual bool setPersonVorname(int ID, QString Vorname)=0;
};

#endif // DATENQUELLE_H
