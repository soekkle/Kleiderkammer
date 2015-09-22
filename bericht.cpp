#include "bericht.h"

Bericht::Bericht(DatenQuelle *Daten)
{
    this->Daten=Daten;
    Typ=1;
}

Bericht::~Bericht()
{
}

QString Bericht::generiereKammerListe()
{
    QString HTML="<!DOCTYPE html >\n<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"de-DE\" prefix=\"og: http://ogp.me/ns# fb: http://ogp.me/ns/fb#\">\n<head >\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n<link rel=\"stylesheet\" href=\"style.css\">";
    HTML=HTML.append("<title> Inventurliste von %1 </title>").arg(QDate::currentDate().toString("dd.MM.yyyy"));
    HTML.append("</head><body>");
    HTML=HTML.append("<h1> Inventurliste von %1 <h1>").arg(QDate::currentDate().toString("dd.MM.yyyy"));
    Kleidungstypentabelle *Typen=Daten->getKleidungstypen();
    for (int i=0;i<Typen->Anzahl;++i)
    {
        HTML=HTML.append("<h2>%1. %2</h2>").arg(QString::number(i+1),Typen->Name[i]);
        int Typ=Typen->ID[i];
        GroessenTabelle *Groessen=Daten->getGroessen(&Typ,1);
        KleiderTabelle *Kleider=Daten->getKleiderinKammer(Typ,-1);
        HTML=HTML.append(QString::fromUtf8("<p>In der Kliderkammer sind %1 %2 in %3 Größen vohanden.</p><table border=\"1\"><tr><th>Göße</th><th>Anzahl</th><th>Nummer</th></tr>")).arg(
                    QString::number(Kleider->Anzahl),Typen->Name[i],QString::number(Groessen->Anzahl));
        for(int j=0;j<Groessen->Anzahl;++j)
        {
            delete Kleider;
            Kleider=Daten->getKleiderinKammer(Typ,Groessen->IDs[j]);
            HTML=HTML.append(QString::fromUtf8("<tr><td>%1</td><td>%2</td><td>")).arg(
                        Groessen->Namen[j],QString::number(Kleider->Anzahl));
            for (int k=0;k<Kleider->Anzahl;++k)
            {
                if (k>0)
                    HTML=HTML.append(", %1").arg(Kleider->Nummer[k]);
                else
                    HTML.append(QString::number(Kleider->Nummer[k]));
            }
            HTML.append("</td></tr>");
        }
        delete Kleider;
        Kleider=Daten->getKleiderinKammer(Typ,0);
        if (Kleider->Anzahl>0)
        {
            HTML=HTML.append("<tr><td>Unbekannt</td><td>%1</td><td>").arg(Kleider->Anzahl);
            for (int k=0;k<Kleider->Anzahl;++k)
            {
                if (k>0)
                    HTML=HTML.append(", %1").arg(Kleider->Nummer[k]);
                else
                    HTML.append(QString::number(Kleider->Nummer[k]));
            }
        }
        HTML.append("</td></tr></table>");
        delete Groessen;
        delete Kleider;
    }
    delete Typen;
    return HTML.append("</body></html>");
}

/*!
 * \brief Bericht::generierenPersonenListe Generiert eien HTML-Webseite mit einer Tabelle in, der die Nummer der ausgeliehnenen
 * Kleidungsstücke für die verschiedenen Personen Eingetragen sind.
 * \param Gruppe ID der Gruppe Für die der Bereicht generiert werden soll.
 * \return  Gibt HTML Text des Reports zurück.
 */
QString Bericht::generierenPersonenListe(int Gruppe)
{
    QString HTML="<!DOCTYPE html >\n<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"de-DE\" prefix=\"og: http://ogp.me/ns# fb: http://ogp.me/ns/fb#\">\n<head >\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n<link rel=\"stylesheet\" href=\"style.css\">";
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
    int anz=1;
    if (Gruppe==0)
        anz=0;
    PersonenTabelle *Personen=Daten->getPersonen(&Gruppe,anz);
    for (int i=0;i<Personen->Anzahl;++i)
    {
        if (i%2==0)
            HTML.append("<tr class=\"UZeile\">");
        else
            HTML.append("<tr class=\"GZeile\">");
        HTML=HTML.append("<td class=\"Name\">%1</td><td class=\"Name\">%2</td>").arg(Personen->Nachname[i],Personen->Vorname[i]);
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
