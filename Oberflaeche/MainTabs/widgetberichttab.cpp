/*
 * Copyright (C) 2016 Sören Krecker
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

#include "widgetberichttab.h"
#include "ui_widgetberichttab.h"

WidgetBerichtTab::WidgetBerichtTab(DatenQuelle *Daten,QString Ort, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WidgetBerichtTab)
{
    ui->setupUi(this);
    // Estellen und Inizalisiren der Objekte
    this->Daten=Daten;
    this->Ort=Ort;
    Drucken=new Bericht(Daten,Ort);

    // Erstellen der Verbindungen
    connect(ui->pushButton_BeAn,SIGNAL(clicked()),this,SLOT(BerichtAnzeigen()));
#ifndef NOPRINT
    connect(ui->pushButton_BeDr,SIGNAL(clicked()),this,SLOT(BerichtDrucken()));
#endif
    connect(ui->pushButton_BeSp,SIGNAL(clicked()),this,SLOT(BerichtSpeichern()));
    connect(ui->radioButton,SIGNAL(clicked()),this,SLOT(RadiobuttomCilcked()));
    connect(ui->radioButton_2,SIGNAL(clicked()),this,SLOT(RadiobuttomCilcked()));
    connect(ui->groupBox_BeTyp,SIGNAL(clicked(bool)),this,SLOT(Groupchecked(bool)));
#ifdef NOPRINT
    ui->pushButton_BeDr->close();
#endif
}

WidgetBerichtTab::~WidgetBerichtTab()
{
    delete Drucken;
    delete ui;
}

/*!
 * \brief MainWindow::BerichtSpalten liefert einen Vektor mit den für den aktuellen Bericht ausgewählten Spalten.
 * Wenn alle die option alle Spalten gewählt wurde wird ein leerer Vektor zurückgegeben.
 * \return Vector mit den ID der Kleidertypen.
 */
QVector<int> WidgetBerichtTab::BerichtSpalten()
{
    QVector<int> Spalten;
    if (ui->groupBox_BeTyp->isChecked())
    {
        for (int i=0;i<CheckBoxBeType.size();++i)
        {
            if (CheckBoxBeType[i]->isChecked())
                Spalten.append(Daten->getKleidungsTypID(CheckBoxBeType[i]->text()));
        }
    }
    return Spalten;
}

/*!
 * \brief MainWindow::BerichtAnzeigen Zeigt den Bericht in der webView des Tabs an. Es werden die in der Oberfläche gewälten einstellungen an die
 * entsprechende Funktion der Klasse Bericht übergeben.
 */
void WidgetBerichtTab::BerichtAnzeigen()
{
    QUrl Url=QUrl::fromLocalFile(Ort);
    int Gruppe=ui->comboBox_BeJF->itemData(ui->comboBox_BeJF->currentIndex()).toInt();
    if (ui->radioButton->isChecked())
        ui->webView->setHtml(Drucken->generiereKammerListe(),Url);
    if (ui->radioButton_2->isChecked())
        ui->webView->setHtml(Drucken->generierenPersonenListe(Gruppe,BerichtSpalten()),Url);
}

#ifndef NOPRINT
/*!
 * \brief MainWindow::BerichtDrucken Starte den Vorgang des Drucken eines Bereichtes. Es wird de standad Dialog mit den Druckereinstellungen angezeigt.
 */
void WidgetBerichtTab::BerichtDrucken()
{
    QPrinter Drucker;
    QUrl Url=QUrl::fromLocalFile(Ort);
    QPrintDialog DruckDialog(&Drucker,this);
    if (DruckDialog.exec()!= QDialog::Accepted)//Öffnet den Druckendialog und prüft ob gedruckt werden soll.
        return ;
    int Gruppe=ui->comboBox_BeJF->itemData(ui->comboBox_BeJF->currentIndex()).toInt();
    QString HTML="";
    if (ui->radioButton->isChecked())
        HTML=Drucken->generiereKammerListe();
    if (ui->radioButton_2->isChecked())
        HTML=Drucken->generierenPersonenListe(Gruppe,BerichtSpalten());
    QWebView* Flaeche=new QWebView;
    QEventLoop Loop;
    connect(Flaeche,SIGNAL(loadFinished(bool)),&Loop,SLOT(quit()));
    Flaeche->setHtml(HTML,Url);
    Loop.exec();
    Flaeche->print(&Drucker);//rendert den Bericht in einer nicht sichtbaren QWebview und gibt inh an den Drucker weiter.
    delete Flaeche;
}
#endif

/*!
 * \brief MainWindow::BerichtSpeichern Speicher den Generierten Bericht in einem HTML Dokument.
 */
void WidgetBerichtTab::BerichtSpeichern()
{
    int Gruppe=ui->comboBox_BeJF->itemData(ui->comboBox_BeJF->currentIndex()).toInt();
    QString Datei=QFileDialog::getSaveFileName(this,tr("Bericht Speichern"),QString(),tr("Webseite(*.html)"));
    // Prüft ob ein eine Dateiendung entahlten ist. Wenn nicht wird sie Gesetzt
    if (!Datei.endsWith(".html",Qt::CaseInsensitive))
        Datei.append(".html");
    QFile HDD_Datei(Datei);
    if (!HDD_Datei.open(QIODevice::WriteOnly | QIODevice::Text))//Prüft ob die Datei geöffnet werden kann.
        return;
    QTextStream HTML(&HDD_Datei);
    HTML.setCodec("UTF-8");
    Drucken->CSSextern=false;
    if (ui->radioButton->isChecked())
        HTML<<Drucken->generiereKammerListe();
    if (ui->radioButton_2->isChecked())
        HTML<< Drucken->generierenPersonenListe(Gruppe,BerichtSpalten());
    HDD_Datei.close();
    Drucken->CSSextern=true;
}

void WidgetBerichtTab::Groupchecked(bool checked)
{
    for (int i=0;i<CheckBoxBeType.size();++i)
    {
        CheckBoxBeType[i]->setHidden(!checked);
    }
}

void WidgetBerichtTab::DatenGeaendert()
{
    // Neufüllen der Combobox mit den Namen der Gruppen
    ui->comboBox_BeJF->clear();
    ui->comboBox_BeJF->addItem("Alle",QVariant(0));
    JugendFeuerwehrTabelle *JfDaten=Daten->getJugendfeuerwehr();
    for (int i=0;i<JfDaten->Anzahl;++i)
    {
        ui->comboBox_BeJF->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
    }
    delete JfDaten;

    for (int i=0;i<CheckBoxBeType.size();++i)
    {
        ui->gridLayout_4->removeWidget(CheckBoxBeType[i]);
        delete CheckBoxBeType[i];
    }
    CheckBoxBeType.clear();
    // Leuaufbauen der Checkboxen
    for (int i=0;i<CheckBoxBeType.size();++i)
    {
        ui->gridLayout_4->removeWidget(CheckBoxBeType[i]);
        delete CheckBoxBeType[i];
    }
    CheckBoxBeType.clear();
    Kleidungstypentabelle *KleiTyp=Daten->getKleidungstypen();
    for (int i=0;i<KleiTyp->Anzahl;++i)
    {
        QCheckBox *Box=new QCheckBox(KleiTyp->Name[i],this);
        Box->setHidden(!ui->groupBox_BeTyp->isChecked());
        ui->gridLayout_4->addWidget(Box,i/4,i%4);
        CheckBoxBeType.append(Box);
    }
    delete KleiTyp;
}

void WidgetBerichtTab::RadiobuttomCilcked()
{
    if (ui->radioButton->isChecked())
    {
        ui->comboBox_BeJF->setEnabled(false);
        ui->groupBox_BeTyp->setChecked(false);
        ui->groupBox_BeTyp->setEnabled(false);
        Groupchecked(false);
    }
    else
    {
        ui->comboBox_BeJF->setEnabled(true);
        ui->groupBox_BeTyp->setEnabled(true);
    }
}
