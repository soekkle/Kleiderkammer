#include "bericht.h"

Bericht::Bericht(DatenQuelle *Daten, QWidget* parent)
{
    this->Daten=Daten;
    Typ=1;
}

Bericht::~Bericht()
{
}

QString Bericht::generiereKammerListe()
{
    QString HTML="<html><head>";
    return HTML.append("</body></html>");
}

QString Bericht::generierenPersonenListe(int Gruppe)
{
    QString HTML="<html><head>";
    QString GName="";
    {
        JugendFeuerwehrTabelle *GruppenNamen=Daten->getJugendfeuerwehr();
        for (int i=0;i<GruppenNamen->Anzahl;++i)
        {
            if (GruppenNamen->ID[i]==Gruppe)
                GName=GruppenNamen->Name[i];
        }
        delete GruppenNamen;
    }
    HTML.append("<title> Kleidungstabelle für").append(GName).append("</title>");
    HTML.append("</head><body>");
    HTML=HTML.append("<h1>Kleidungstabelle %1 vom %2 </h1>").arg(GName,QDate::currentDate().toString("dd.MM.yyyy"));
    HTML.append("<table><thead><td>Nachname</td><td>Vorname</td>");
    QVector<int> TypenListe;
    {
       Kleidungstypentabelle *Typen=Daten->getKleidungstypen();
       for (int i=0;i<Typen->Anzahl;++i)
       {
           TypenListe.append(Typen->ID[i]);
           HTML=HTML.append("<th>%1</th>").arg(Typen->Name[i]);
       }
       delete Typen;
    }
    int anzTypen=TypenListe.count();
    PersonenTabelle *Personen=Daten->getPersonen(&Gruppe,1);
    for (int i=0;i<Personen->Anzahl;++i)
    {
        HTML=HTML.append("<tr><td>%1</td><td>%2</td>").arg(Personen->Nachname[i],Personen->Vorname[i]);
        for (int j=0;j<anzTypen;++j)
        {
            KleiderTabelle *Kleidung=Daten->getKleidervonPerson(Personen->ID[i],TypenListe[j]);
            HTML.append("<td>");
            for (int k=0;k<Kleidung->Anzahl;++k)
            {
                if(k>0)
                    HTML=HTML.append(",<br/>%1").arg(QString::number(Kleidung->Nummer[k]));
                else
                    HTML.append(QString::number(Kleidung->Nummer[k]));
            }
            delete Kleidung;
            HTML.append("</td>");
        }
        HTML.append("</tr>");
    }
    delete Personen;
    HTML.append("</thead><tbody>");
    HTML.append("</tbody></table>");
    return HTML.append("</body></html>");
}
