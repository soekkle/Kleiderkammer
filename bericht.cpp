/*
 * Copyright (C) 2015 Sören Krecker
 *
 * This file is part of Kleiderkammer.
 *
 * Kleiderkammer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation version 3 of the License.
 *
 * Kleiderkammer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Kleiderkammer.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Diese Datei ist Teil von Kleiderkammer.
 *
 * Kleiderkammer ist Freie Software: Sie können es unter den Bedingungen
 * der GNU Affero General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz, weiterverbreiten und/oder modifizieren.
 *
 * Kleiderkammer wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN
 * ZWECK.Siehe die GNU Affero General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU Affero General Public License zusammen mit
 * diesem Programm erhalten haben. Wenn nicht, siehe
 * <http://www.gnu.org/licenses/>.
*/

#include "bericht.h"

Bericht::Bericht(DatenQuelle *Daten, QString Ort)
{
    this->Daten=Daten;
    FileDir=Ort;
    CSSFile="style.css";
    CSSextern=false;
}

Bericht::~Bericht()
{
}

QString Bericht::generiereKammerListe()
{
    QString HTML="<!DOCTYPE html >\n<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"de-DE\" prefix=\"og: http://ogp.me/ns# fb: http://ogp.me/ns/fb#\">\n<head >\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n";
    HTML.append(CSSHeader());
    HTML.append("<title> Inventurliste von %1 </title>").arg(QDate::currentDate().toString("dd.MM.yyyy"));
    HTML.append("</head><body>");
    HTML=HTML.append("<h1> Inventurliste von %1 <h1>").arg(QDate::currentDate().toString("dd.MM.yyyy"));
    Kleidungstypentabelle *Typen=Daten->getKleidungstypen();
    for (int i=0;i<Typen->Anzahl;++i)
    {
        HTML=HTML.append("<h2>%1. %2</h2>").arg(QString::number(i+1),Typen->Name[i]);
        int Typ=Typen->ID[i];
        GroessenTabelle *Groessen=Daten->getGroessen(&Typ,1);
        KleiderTabelle *Kleider=Daten->getKleiderinKammer(Typ,-1);
        HTML=HTML.append(QString::fromUtf8("<p>In der Kleiderkammer sind %1 %2 in %3 Größen vohanden.</p><table border=\"1\"><thead><th style=\"width:20%\">Göße</th><th style=\"width:15%\">Anzahl</th><th>Nummer</th></thead><tbody>")).arg(
                    QString::number(Kleider->Anzahl),Typen->Name[i],QString::number(Groessen->Anzahl));
        for(int j=0;j<Groessen->Anzahl;++j)
        {
            delete Kleider;
            Kleider=Daten->getKleiderinKammer(Typ,Groessen->IDs[j]);
            if (j%2==0)
                HTML.append("<tr class=\"UZeile\">");
            else
                HTML.append("<tr class=\"GZeile\">");
            HTML=HTML.append(QString::fromUtf8("<td class=\"Name\">%1</td><td>%2</td><td>")).arg(
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
            if (Groessen->Anzahl%2==0)
                HTML.append("<tr class=\"UZeile\">");
            else
                HTML.append("<tr class=\"GZeile\">");
            HTML=HTML.append("<td class=\"Name\">Unbekannt</td><td>%1</td><td>").arg(Kleider->Anzahl);
            for (int k=0;k<Kleider->Anzahl;++k)
            {
                if (k>0)
                    HTML=HTML.append(", %1").arg(Kleider->Nummer[k]);
                else
                    HTML.append(QString::number(Kleider->Nummer[k]));
            }
        }
        HTML.append("</td></tr></tbody></table>");
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
QString Bericht::generierenPersonenListe(int Gruppe, QVector<int> TypenListe)
{
    QString HTML="<!DOCTYPE html >\n<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"de-DE\" prefix=\"og: http://ogp.me/ns# fb: http://ogp.me/ns/fb#\">\n<head >\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n";
    HTML.append(CSSHeader());
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
    {
        Kleidungstypentabelle *Typen=Daten->getKleidungstypen();
        if (TypenListe.isEmpty())
        {
            for (int i=0;i<Typen->Anzahl;++i)
            {
                TypenListe.append(Typen->ID[i]);
                HTML=HTML.append("<th>%1</th>").arg(Typen->Name[i]);
            }
        }
        else
        {
            for (int i=0;i<TypenListe.size();++i)
            {
                int j=Typen->ID.indexOf(TypenListe[i]);
                HTML=HTML.append("<th>%1</th>").arg(Typen->Name[j]);
            }
        }
        delete Typen;
    }
    HTML.append("</thead><tbody>");
    int anzTypen=TypenListe.count();
    int GAnz=1;
    if (Gruppe==0)
        GAnz=0;
    PersonenTabelle *Personen=Daten->getPersonen(&Gruppe,GAnz);
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
            if (j%2==0)
                HTML.append("<td class=\"GSpalte\">");
            else
                HTML.append("<td class=\"USpalte\">");
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

QString Bericht::CSSHeader()
{
    QString header="";
    if (CSSextern)
    {
        header="<link rel=\"stylesheet\" href=\"%1\">\n";
        header=header.arg(CSSFile);
    }
    else
    {
        header="<style>";
        QFile CSSText(CSSFile);
        QString OrtCSS=FileDir;
        OrtCSS.append(CSSFile);
        QFile CSSOrt(OrtCSS);
        if (CSSText.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream CSS(&CSSText);
            header.append(CSS.readAll());
            CSSText.close();
        }
        else if (CSSOrt.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream CSS(&CSSOrt);
            header.append(CSS.readAll());
            CSSOrt.close();
        }
        header.append("</style>");
    }
    return header;
}

/*!
 * \brief Bericht::setCSS setzt den pfard auf die zu verwendene CSS-Datei. Vorher wird geprüft ob die Datei vorhanden ist.
 * Wenn sie nicht vorhanden ist, so wird die alte Datei behalten und false zurückgegeben.
 * \param Ort Ort an der die CSS-Datei liegt.
 * \return Rückgabe ob das sätzen Erfolgreich war.
 */
bool Bericht::setCSS(QString Ort)
{
    QFile CSS(Ort);
    if (CSS.exists())
    {
        CSSFile=Ort;
        return true;
    }
    QString OrtE=FileDir;
    OrtE.append(Ort);
    CSS.setFileName(OrtE);
    if (CSS.exists())
    {
        CSSFile=Ort;
        return true;
    }
    return false;
}
