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

#include "kleidersuche.h"
#include "ui_kleidersuche.h"

KleiderSuche::KleiderSuche(DatenQuelle *Daten, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KleiderSuche)
{
    ui->setupUi(this);
    this->Daten=Daten;
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slotSuchen()));
}

KleiderSuche::~KleiderSuche()
{
    delete ui;
}
/*!
 * \brief KleiderSuche::slotSuchen Fürt die suche nach einen Kleidungsstück mit der eingebenen Nummer aus. Und füllt
 *  das Formular mit den Daten.
 */
void KleiderSuche::slotSuchen()
{
    int Nummer=ui->lineEdit->text().toInt();
    int Leihen;
    QString Typ,Name,Gruppe,Groesse,Bemerkung;
    QDate Anschaffung;
    // Prüft ob das Kleidungsstück vorhanden ist.
    if (Daten->getKleidungsInfoByNummer(Nummer,&Typ,&Groesse,&Anschaffung,&Name,&Gruppe,&Bemerkung,&Leihen)>0)
    {
        // Setzen der Felder im Formular
        ui->label_Typ->setText(Typ);
        ui->label_Goesse->setText(Groesse);
        ui->label_Alte->setText(Anschaffung.toString("dd.MM.yyyy"));
        ui->label_Bemerkung->setText(Bemerkung);
        if (Name.isEmpty())
        {
            ui->label_Ort->setText(QString::fromUtf8("Kleidungsstück ist in der Kleiderkammer."));
        }
        else {
            Name.append(" - ").append(Gruppe);
            ui->label_Ort->setText(Name);
        }
    }
    else
    {
        // Leeren der Felder im Formular
        ui->label_Typ->setText("");
        ui->label_Goesse->setText("");
        ui->label_Alte->setText("");
        ui->label_Bemerkung->setText("");
        ui->label_Ort->setText("");
        // Ausgabe einer Infomeldung.
        QMessageBox::information(this,QString::fromUtf8("Kleidungsstück nicht Vohanden."),
                                 QString::fromUtf8("Es gibt kein Kleidungsstück mit der Nummer %1 in der Datenkank.").arg(Nummer));
    }
}
