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
    //! Gibt den HTML-Code für die Personenliste für die Angegebene Gruppe zurück
    QString generierenPersonenListe(int Gruppe);
    //! Gibt den HTML-Code für die Inventurliste zurück.
    QString generiereKammerListe();

private:
    int Typ;
    DatenQuelle *Daten;
};

#endif // BERICHT_H
