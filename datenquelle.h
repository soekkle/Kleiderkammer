#ifndef DATENQUELLE_H
#define DATENQUELLE_H

#include <QString>
#include <QVector>
#include <QDateTime>

//! Hilfsklasse zur Ausgabe der Größen.
class GroessenTabelle
{
public:
    int Anzahl;
    QVector<int> IDs;
    QVector<QString> Namen;
    QVector<QString> Typ;
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

class KleiderTabelle
{
    //! Klasse zum zwischenspeichern von Abfrageergebnissen mit Kleidungsstücken.
public:
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

class PersonenTabelle
{
public:
    int Anzahl;
    QVector<int> ID;
    QVector<QString> Vorname;
    QVector<QString> Nachname;
    QVector<QString> JugendFeuerwehr;
};

class DatenQuelle
{
public:
    DatenQuelle();
    virtual ~DatenQuelle();
    //! Fügt eine Größe hinzu.
    virtual int addGroesse(QString Groesse,int Typ)=0;
    //! Fügt einen JugendFeuerweher hinzu.
    virtual int addJugendfeuerwehr(QString Name)=0;
    virtual int addKleiderstueck(int Typ,int Groesse,int Nummer=-1)=0;
    virtual int addKleidungstyp(QString Name, int AnfangsNummer, int Endnummer)=0;
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
    //! Liefert die Informationen zu einen Kleidungsstück
    virtual int getKleidungsInfoByNummer(int Nummer, QString* Typ, QString *Groesse, QDate *Datum, QString *Traeger, QString *Gruppe, QString *Bemerkung, int *Anzahl)=0;
    virtual Kleidungstypentabelle *getKleidungstypen()=0;
    //! Liefert die ID eines Kleidungstyps.
    virtual int getKleidungsTypID(QString Typ)=0;
    virtual void getNummerBereich(int TypID,int *Anfang,int* Ende)=0;
    virtual PersonenTabelle *getPersonen(int *JFFilter,int JFans)=0;
    virtual bool KleidungsstueckzuordnenbyID(int ID,int Traeger)=0;
    virtual bool removeGrosse(int ID)=0;
    virtual bool removeJugendferweher(int ID)=0;
    virtual bool removeKleidungsstueck(int ID)=0;
    virtual bool removeKleidungstyp(int ID)=0;
    virtual bool removePerson(int ID)=0;
    virtual void rueckgabeKleidungsstueck(int ID)=0;
    virtual bool setKleidungsGroesse(int ID, int GroesseID)=0;
    virtual bool setKleidungsKommentar(int ID, QString Kommentar)=0;
    virtual bool setRangGroesse(int ID,int Rang)=0;
};

#endif // DATENQUELLE_H
