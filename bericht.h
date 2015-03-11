#ifndef BERICHT_H
#define BERICHT_H

#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDateTime>
#include "datenquelle.h"



class Bericht
{
public:
    Bericht(DatenQuelle *Daten);
    ~Bericht();
    QString generierenPersonenListe(int Gruppe);
    QString generiereKammerListe();

private:
    int Typ;
    DatenQuelle *Daten;
};

#endif // BERICHT_H
