#ifndef DATENQUELLE_H
#define DATENQUELLE_H

#include <QString>

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
    virtual int getIDByKleidungsNummer(int Nummer)=0;
    virtual bool KleidungsstueckzuordnenbyID(int ID,int Traeger)=0;
    virtual bool removeGrosse(int ID)=0;
    virtual bool removeJugendferweher(int ID)=0;
    virtual bool removeKleidungstyp(int ID)=0;
    virtual bool removePerson(int ID)=0;
};

#endif // DATENQUELLE_H
