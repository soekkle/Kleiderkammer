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
    QString HTML="<html><head> <link rel=\"stylesheet\" href=\"style.css\">";
    HTML=HTML.append("<title> Inventurliste von %1 </title>").arg(QDate::currentDate().toString("dd.MM.yyyy"));
    HTML.append("</head><body>");
    HTML=HTML.append("<h1> Inventurliste von %1 <h1>").arg(QDate::currentDate().toString("dd.MM.yyyy"));
    Kleidungstypentabelle *Typen=Daten->getKleidungstypen();
    for (int i=0;i<Typen->Anzahl;++i)
    {
        HTML=HTML.append("<h2>%1. %2</h2>").arg(QString::number(i+1),Typen->Name[i]);
        int Typ=Typen->ID[i];
        GroessenTabelle *Groessen=Daten->getGroessen(&Typ,1);
        KleiderTabelle *Kleider=Daten->getKleiderinKammer(Typ,0);
        HTML=HTML.append(QString::fromUtf8("<p>In der Kliderkammer sind %1 %2 in %3 Größen.</p>")).arg(QString::number(Kleider->Anzahl)
                                                                                    ,Typen->Name[i],QString::number(Groessen->Anzahl));
        for(int j=0;j<Groessen->Anzahl;++j)
        {
            delete Kleider;
            Kleider=Daten->getKleiderinKammer(Typ,Groessen->IDs[j]);
            HTML=HTML.append(QString::fromUtf8("<h3>%5.%6 %1</h3><p>Es sind %3 %2 in Größe %1 in der Kleiderkammer mit den Nummer:</p><p>")).arg(
                        Groessen->Namen[j],Typen->Name[i],QString::number(Kleider->Anzahl),QString::number(i+1),QString::number(j+1));
            for (int k=0;k<Kleider->Anzahl;++k)
            {
                if (k>0)
                    HTML=HTML.append(", %1").arg(Kleider->Nummer[k]);
                else
                    HTML.append(QString::number(Kleider->Nummer[k]));
            }
            HTML.append("</p>");
        }
        delete Groessen;
        delete Kleider;
    }
    delete Typen;
    return HTML.append("</body></html>");
}

QString Bericht::generierenPersonenListe(int Gruppe)
{
    QString HTML="<html><head><link rel=\"stylesheet\" href=\"style.css\">";
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
    HTML=HTML.append("<title> Kleidungstabelle für %1</title>").arg(GName);
    HTML.append("</head><body>");
    HTML=HTML.append("<h1>Kleidungstabelle %1 vom %2 </h1>").arg(GName,QDate::currentDate().toString("dd.MM.yyyy"));
    HTML.append("<table border=\"1\"><thead><th>Nachname</th><th>Vorname</th>");
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
    HTML.append("</thead><tbody>");
    int anzTypen=TypenListe.count();
    PersonenTabelle *Personen=Daten->getPersonen(&Gruppe,1);
    for (int i=0;i<Personen->Anzahl;++i)
    {
        HTML=HTML.append("<tr><td class=\"Name\">%1</td><td class=\"Name\">%2</td>").arg(Personen->Nachname[i],Personen->Vorname[i]);
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
    HTML.append("</tbody></table>");
    return HTML.append("</body></html>");
}
