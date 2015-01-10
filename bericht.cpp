#include "bericht.h"

Bericht::Bericht(DatenQuelle *Daten, QWidget* parent)
{
    this->Daten=Daten;
    DruckDialog=new QPrintDialog(&Drucker,parent);
    Typ=1;
}

Bericht::~Bericht()
{
    delete DruckDialog;
}

bool Bericht::generiereKammerListe()
{
    if (DruckDialog->exec()!= QDialog::Accepted)
        return true;
    Kleidungstypentabelle* KTypen=Daten->getKleidungstypen();
    QPainter Ausgabe(&Drucker);
    SeitenLayout("Inventarliste",1,1,&Ausgabe);
    delete KTypen;
    return true;
}

bool Bericht::generierenPersonenListe(int Gruppe)
{
    if (DruckDialog->exec()!= QDialog::Accepted)
        return true;
    int filteranz=1;
    if (Gruppe==0)
        filteranz=0;
    PersonenTabelle* Personen=Daten->getPersonen(&Gruppe,filteranz);
    QPainter Ausgabe(&Drucker);
    SeitenLayout("Kleidungsliste",1,1,&Ausgabe);
    delete Personen;
    return true;
}

void Bericht::SeitenLayout(QString Name, int SeitenNummer, int SeitenAnzahl,QPainter *Seite)
{
    double faktorhoehe=Drucker.height()/(double)Drucker.heightMM();
    Seite->drawText(0,12*faktorhoehe,Drucker.width(),20*faktorhoehe,Qt::AlignHCenter|Qt::AlignVCenter,Name);
    QString Text="Seite %1 von %2";
    Text=Text.arg(SeitenNummer).arg(SeitenAnzahl);
    Seite->drawText(0,Drucker.height()-12*faktorhoehe,Drucker.width(),20*faktorhoehe,Qt::AlignHCenter|Qt::AlignVCenter,Text);
}
