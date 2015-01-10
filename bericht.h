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
    Bericht(DatenQuelle *Daten, QWidget *parent=0);
    ~Bericht();
    bool generierenPersonenListe(int Gruppe);
    bool generiereKammerListe();

private:
    int Typ;
    DatenQuelle *Daten;
    QPrinter Drucker;
    QPrintDialog *DruckDialog;
    void SeitenLayout(QString Name, int SeitenNummer, int SeitenAnzahl, QPainter *Seite);
};

#endif // BERICHT_H
