#ifndef DATENQUELLE_H
#define DATENQUELLE_H

#include <QString>

class DatenQuelle
{
public:
    DatenQuelle();
    //virtual ~DatenQuelle();
    virtual int addKleiderstueck(int Typ,int Groesse,int Nummer=-1)=0;
    virtual int addPerson(QString Nachname, QString Vorname,int Gruppe)=0;
    virtual int freieNummer(int Typ)=0;
};

#endif // DATENQUELLE_H
