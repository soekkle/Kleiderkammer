/*
 * Copyright (C) 2014,2015 Sören Krecker
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PersonenID=0;
//#if __linux__||__unix__
    Ort=QDir::homePath();//Setzt den Pfard der Anwendung unter Linux
    Ort=Ort.append("/.config");
/*#elif __WIN32__||_MSC_VER
    PWSTR *localAppData=new PWSTR[256];
    _GUID Local;//Ersetzrt die _GUID FOLDERID_LocalAppData ist nicht schön aber läuft.
    Local.Data1=0xF1B32785;
    Local.Data2=0x6FBA;
    Local.Data3=0x4FCF;
    Local.Data4[0]=0x9D;
    Local.Data4[1]=0x55;
    Local.Data4[2]=0x7B;
    Local.Data4[3]=0x8E;
    Local.Data4[4]=0x7F;
    Local.Data4[5]=0x15;
    Local.Data4[6]=0x70;
    Local.Data4[7]=0x91;
    SHGetKnownFolderPath(Local,0,NULL,localAppData);
    Ort=QString::fromWCharArray(*localAppData);//setzt den Pfard der Anwendung unter Windows
    delete localAppData;

#endif*/
    Ort.append("/Kleiderkammer/");
    {
        QDir Appdir(Ort);
        if(!Appdir.exists())
        {
            Appdir.mkpath(Ort);
        }
    }
    QString Ort=this->Ort;
    Ort.append("Daten.sqlite");
    std::clog <<Ort.toStdString()<<std::endl;
    Daten=new SQLiteQuelle(Ort);//Stellt die verbindung zur Datenbank her.
    Typen=new KleidungsTypenVerwaltung(Daten,this);//Erzeugt die Modelle zur Anzeige der Daten.
    Gruppen=new Gruppenverwaltung(Daten,this);
    KleiderInfoSuchen=new KleiderSuche(Daten,this);
    Kleidungstuecke = new KleidungsTableview(Daten,0,this);
    KleiderAus = new KleidungsTableview(Daten,0,this);
    PerKleider = new KleidungsTableview(Daten,1,this);
    ProKleidungstuecke.setSourceModel(Kleidungstuecke);//Verbinden Des Proxiemodell mit dem Datenmodell.
    ComboBox=new ComboboxGroessenDelegate(Daten,this);
    ui->tableKleidung->setModel(&ProKleidungstuecke);//Setzt die Modelle zur Anzeige der Daten.
    ui->tableKleidung->setItemDelegateForColumn(2,ComboBox);// Setzen der Combobox für die Größen
    ProPersonen.setSourceModel(&Personen);
    ui->tablePersonen->setModel(&ProPersonen);
    ProKleiderAus.setSourceModel(KleiderAus);
    ui->table_Kleileihen->setModel(&ProKleiderAus);
    ui->table_Kleileihen->setItemDelegateForColumn(2,ComboBox);
    ProPerKleider.setSourceModel(PerKleider);
    ui->tableView_KleiPerson->setModel(&ProPerKleider);
    ui->tableView_KleiPerson->setItemDelegateForColumn(2,ComboBox);
    ComboboxFuellen();//Inizalisiert alle Comboboxen.
    Drucken=new Bericht(Daten,this->Ort);
    //Stellt alle Eventverbindungen her.
    connect(ui->actionKleidungstypen_verwalten,SIGNAL(triggered()),Typen,SLOT(exec()));
    connect(Typen,SIGNAL(datenGeaendert()),this,SLOT(ComboboxFuellen()));
    connect(ui->actionGruppen_Verwalten,SIGNAL(triggered()),Gruppen,SLOT(exec()));
    connect(Gruppen,SIGNAL(datenGeaendert()),this,SLOT(ComboboxFuellen()));
    connect(KleiderInfoSuchen,SIGNAL(PersonGewaehlt(int)),this,SLOT(ZeigePersonKleider(int)));
    connect(ui->comboBoxPerJFFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(ComboboxPerJFFilterGewahlt(int)));
    connect(ui->lineEditSuchName,SIGNAL(textChanged(QString)),this,SLOT(LineEditSuchNameChange(QString)));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(PersonHinClicked()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(PersonHinCancel()));
    connect(ui->comboBoxBekFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(KleidunginKammerAnzeigen(int)));
    connect(ui->comboBoxBeTypEin,SIGNAL(currentIndexChanged(int)),this,SLOT(Kleidungstypgewaehlt(int)));
    connect(ui->buttonBox_2,SIGNAL(accepted()),this,SLOT(KleidungHinClicked()));
    connect(ui->buttonBox_2,SIGNAL(rejected()),this,SLOT(KleidungHinCancel()));
    connect(ui->tablePersonen->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(PersonAusgewaehlt(QModelIndex,QModelIndex)));
    connect(ui->comboBox_AusTypFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(AusTypFiltergeaendert(int)));
    connect(ui->comboBox_AusGroFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(AusGroessenFiltergeaendert(int)));
    connect(ui->pushButton_leihen,SIGNAL(clicked()),this,SLOT(Auslehenclicked()));
    connect(ui->comboBox_eigenFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(PerKleidungslistefuellen(int)));
    connect(ui->pushButton_zuruck,SIGNAL(clicked()),this,SLOT(Zurueckgeben()));
    connect(ui->pushButton_BeAn,SIGNAL(clicked()),this,SLOT(BerichtAnzeigen()));
#ifndef NOPRINT
    connect(ui->pushButton_BeDr,SIGNAL(clicked()),this,SLOT(BerichtDrucken()));
#endif
    connect(ui->pushButton_BeSp,SIGNAL(clicked()),this,SLOT(BerichtSpeichern()));
    connect(ui->radioButton,SIGNAL(clicked()),this,SLOT(RadiobuttomCilcked()));
    connect(ui->radioButton_2,SIGNAL(clicked()),this,SLOT(RadiobuttomCilcked()));
    connect(ui->groupBox_BeTyp,SIGNAL(clicked(bool)),this,SLOT(Groupchecked(bool)));
    // Verbinden der Mainmenü einträgen
    connect(ui->actionBeenden,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionKleidungsst_ck,SIGNAL(triggered()),KleiderInfoSuchen,SLOT(exec()));
    connect(ui->action_ber,SIGNAL(triggered()),this,SLOT(ZeigeInfo()));
    connect(ui->action_ber_QT,SIGNAL(triggered()),this,SLOT(ZeigeQTInfo()));
    //Verboinden der ContextMenüs
    connect(ui->tablePersonen,SIGNAL(customContextMenuRequested(const QPoint)),this,SLOT(NamenContextMenuEvent(QPoint)));
    connect(ui->tableKleidung,SIGNAL(customContextMenuRequested(const QPoint)),this, SLOT(KleidungContextMenuEvent(QPoint)));

    connect(ui->tablePersonen,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(PersonListeDoubleClicked(const QModelIndex &)));
    //Anlegen der Actionen
    ActionPersonLoeschen= new QAction(QString::fromUtf8("Löschen"),this);
    connect(ActionPersonLoeschen,SIGNAL(triggered()),this,SLOT(PersonLoeschen()));
    ActionKleicungLoeschen=new QAction(QString::fromUtf8("Löschen"),this);
    ActionKleicungLoeschen->setToolTip(QString::fromUtf8("Löscht das Ausgewalte Kleidungsstück."));
    connect(ActionKleicungLoeschen,SIGNAL(triggered()),this,SLOT(KleidungLoeschen()));
#ifdef NOPRINT
    ui->pushButton_BeDr->close();
#endif
}

MainWindow::~MainWindow()
{
    delete ComboBox;
    delete ActionPersonLoeschen;
    delete ui;
    delete Kleidungstuecke;
    delete Daten;
    delete Drucken;
    QString DBAktuell,DBBack;
    DBAktuell=Ort;
    DBAktuell.append("Daten.sqlite");
    DBBack=Ort;
    DBBack.append("Backup/");
    QDir BackDir(DBBack);
    if (!BackDir.exists())
    {
        BackDir.mkpath(DBBack);
    }
    QStringList Datein=BackDir.entryList(QDir::Files,QDir::Name);
    if (Datein.length()>10)
    {
        QString BackAlt=DBBack;
        BackAlt.append(Datein[0]);
        QFile Alt(BackAlt);
        Alt.remove();
    }
    DBBack.append(QDateTime::currentDateTime().toString("yyMMddhhmmss")).append(".db");
    QFile::copy(DBAktuell,DBBack);
}

void MainWindow::AusGroessenFiltergeaendert(int Groesse)
{
    int GroFilter;
    GroFilter=ui->comboBox_AusGroFilter->itemData(Groesse).toInt();
    KleiderAus->setFilterGroesse(GroFilter);
}

void MainWindow::Auslehenclicked()
{
    QModelIndex Index=ProKleiderAus.mapToSource(ProKleiderAus.index(ui->table_Kleileihen->currentIndex().row(),0));
    int id=KleiderAus->getKleidungsId(Index.row());
    if (id==0)
        return;
    int Nummer,Typ,Groesse;
    if (!Daten->getKleidungsInfoByID(id,&Nummer,&Typ,&Groesse))
        return;
    KleiderTabelle *Bestand=Daten->getKleidervonPerson(PersonenID,Typ);
    if (Bestand->Anzahl==1)
    {
        int ruck=QMessageBox::question(this,QString::fromUtf8("Rückgebe"),QString::fromUtf8("Wurde das Kleidungsstück mit der Nummer %1 zurückgegeben?").arg(Bestand->Nummer[0]),
                QMessageBox::Yes|QMessageBox::No);
        if (ruck==QMessageBox::Yes)
        {
            Daten->rueckgabeKleidungsstueck(Bestand->ID[0]);
        }
    }
    delete Bestand;
    Daten->KleidungsstueckzuordnenbyID(id,PersonenID);
    //Ausleihlistefuellen
    KleiderAus->setFilterTyp(ui->comboBox_AusTypFilter->itemData(ui->comboBox_AusTypFilter->currentIndex()).toInt());
    KleiderAus->setFilterGroesse(ui->comboBox_AusGroFilter->itemData(ui->comboBox_AusGroFilter->currentIndex()).toInt());
    PerKleidungslistefuellen(ui->comboBox_eigenFilter->currentIndex());
}


void MainWindow::AusTypFiltergeaendert(int Typ)
{
    ui->comboBox_AusGroFilter->clear();
    if (Typ==0)
    {
        ui->comboBox_AusGroFilter->setEnabled(false);
        KleiderAus->setFilterTyp(0);
        return;
    }
    ui->comboBox_AusGroFilter->addItem("Alle",QVariant(0));
    int Gro=ui->comboBox_AusTypFilter->itemData(Typ).toInt();
    KleiderAus->setFilterTyp(Gro);
    GroessenTabelle *Groessen=Daten->getGroessen(&Gro,1);
    for (int i=0;i<Groessen->Anzahl;++i)
    {
        ui->comboBox_AusGroFilter->addItem(Groessen->Namen[i],QVariant(Groessen->IDs[i]));
    }
    delete Groessen;
    ui->comboBox_AusGroFilter->setEnabled(true);
}

/*!
 * \brief MainWindow::BerichtSpalten liefert einen Vektor mit den für den aktuellen Bericht ausgewählten Spalten.
 * Wenn alle die option alle Spalten gewählt wurde wird ein leerer Vektor zurückgegeben.
 * \return Vector mit den ID der Kleidertypen.
 */
QVector<int> MainWindow::BerichtSpalten()
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

void MainWindow::ComboboxFuellen()
{
    /* Fült zwei Comboboxen mit den Namen der Eingetragenen Gruppen.
       und Speicher die id aus der Datenqelle dazu.*/
    JugendFeuerwehrTabelle *JfDaten=Daten->getJugendfeuerwehr();
    ui->comboBoxPerJFFilter->clear();
    ui->comboBoxPerJFFilter->addItem("Alle",QVariant(0));
    ui->comboBox_BeJF->clear();
    ui->comboBox_BeJF->addItem("Alle",QVariant(0));
    ui->comboPerJFEin->clear();
    for (int i=0;i<JfDaten->Anzahl;++i)
    {
        ui->comboBoxPerJFFilter->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
        ui->comboPerJFEin->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
        ui->comboBox_BeJF->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
    }
    PersonenAnzeigen(0,"");
    delete JfDaten;
    for (int i=0;i<CheckBoxBeType.size();++i)
    {
        ui->gridLayout_4->removeWidget(CheckBoxBeType[i]);
        delete CheckBoxBeType[i];
    }
    CheckBoxBeType.clear();
    /* Fült alle komboboxen die die Typen der Kleidungsstücke benötigen.
       Ebenfalls werden die ID als Daten dazu gespeichert.*/
    ui->comboBoxBekFilter->clear();
    ui->comboBoxBeTypEin->clear();
    ui->comboBox_AusGroFilter->clear();
    ui->comboBox_eigenFilter->clear();
    ui->comboBox_AusTypFilter->clear();
    ui->comboBoxBekFilter->addItem("Alle",QVariant(0));
    ui->comboBox_AusGroFilter->addItem("Alle",QVariant(0));
    ui->comboBox_eigenFilter->addItem("Alle",QVariant(0));
    ui->comboBox_AusTypFilter->addItem("Alle",QVariant(0));
    ui->comboBoxBeTypEin->addItem(QString::fromUtf8("Bitte Wählen"),QVariant(0));
    Kleidungstypentabelle *KleiTyp=Daten->getKleidungstypen();
    for (int i=0;i<KleiTyp->Anzahl;++i)
    {
        ui->comboBoxBekFilter->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
        ui->comboBoxBeTypEin->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
        ui->comboBox_AusTypFilter->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
        ui->comboBox_eigenFilter->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
        QCheckBox *Box=new QCheckBox(KleiTyp->Name[i],this);
        Box->setHidden(!ui->groupBox_BeTyp->isChecked());
        ui->gridLayout_4->addWidget(Box,i/4,i%4);
        CheckBoxBeType.append(Box);
    }
    KleidunginKammerAnzeigen(0);
    delete KleiTyp;
}

/*!
 * \brief MainWindow::BerichtAnzeigen Zeigt den Bericht in der webView des Tabs an. Es werden die in der Oberfläche gewälten einstellungen an die
 * entsprechende Funktion der Klasse Bericht übergeben.
 */

void MainWindow::BerichtAnzeigen()
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
void MainWindow::BerichtDrucken()
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
void MainWindow::BerichtSpeichern()
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

void MainWindow::ComboboxPerJFFilterGewahlt(int Pos)
{
    PersonenAnzeigen(Pos,ui->lineEditSuchName->text());
}

void MainWindow::Groupchecked(bool checked)
{
    for (int i=0;i<CheckBoxBeType.size();++i)
    {
        CheckBoxBeType[i]->setHidden(!checked);
    }
}

/*!
 * \brief MainWindow::KleidungContextMenuEvent Zeigt ein Kontextmenü für das Tableview TabelleKleider an.
 * \param Pos Position Auf der TableView, wo das Contextmenü angezeigt wird,
 */
void MainWindow::KleidungContextMenuEvent(const QPoint Pos)
{
    QMenu menu(this);
    menu.addAction(ActionKleicungLoeschen);
    menu.exec(ui->tableKleidung->viewport()->mapToGlobal(Pos));
}

void MainWindow::KleidungHinCancel()
{
    ui->comboBoxBeTypEin->setCurrentIndex(0);
    ui->comboBoxBeGroEin->setCurrentIndex(0);
    KleidunginKammerAnzeigen(ui->comboBoxBekFilter->currentIndex());
}

void MainWindow::KleidungHinClicked()
{
    if (ui->comboBoxBeTypEin->currentIndex()==0)
        return;
    if (ui->comboBoxBeGroEin->currentIndex()==0)
        return;
    int TypID,GroID,Nummer;
    TypID=ui->comboBoxBeTypEin->itemData(ui->comboBoxBeTypEin->currentIndex()).toInt();
    GroID=ui->comboBoxBeGroEin->itemData(ui->comboBoxBeGroEin->currentIndex()).toInt();
    Nummer=ui->spinBoxBeNumEin->value();
    std::cout<<TypID<<" : "<<GroID<<" : "<<Nummer<<std::endl;
    if (Daten->addKleiderstueck(TypID,GroID,Nummer,false)==-1)
        QMessageBox::warning(this,QString::fromUtf8("Kleidungsstück schon vorhanden"),QString::fromUtf8("Das Kleidungsstück mit der Nummer %1 ist schon angelegt und kann deshalb nicht angelegt werden").arg(Nummer));
    Kleidungstypgewaehlt(ui->comboBoxBeTypEin->currentIndex());
    KleidunginKammerAnzeigen(ui->comboBoxBekFilter->currentIndex());
}

void MainWindow::KleidunginKammerAnzeigen(int Filter)
{
    Kleidungstuecke->setFilterTyp(ui->comboBoxBekFilter->itemData(Filter).toInt());

}

/*!
 * \brief MainWindow::KleidungLoeschen Löscht das ausgewälte Kleidungsstück aus der Kleiderkammer.
 */
void MainWindow::KleidungLoeschen()
{
    //Holen des Orginal Indexes.
    QModelIndex Index=ProKleidungstuecke.mapToSource(ProKleidungstuecke.index(ui->tableKleidung->currentIndex().row(),0));
    int id=Kleidungstuecke->getKleidungsId(Index.row());
    if (QMessageBox::information(this,QString::fromUtf8("Kleidungsstück löschen"),QString::fromUtf8("Sind Sie sich sicher, dass sie das Ausgewälte Kleidungsstück löschen wollen?"),
                                 QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        Daten->removeKleidungsstueck(id);
    KleidunginKammerAnzeigen(ui->comboBoxBekFilter->currentIndex());

}

void MainWindow::Kleidungstypgewaehlt(int Typ)
{
    ui->comboBoxBeGroEin->clear();
    ui->comboBoxBeGroEin->addItem(QString::fromUtf8("Bitte Wählen"),QVariant(-1));
    int TypID=ui->comboBoxBeTypEin->itemData(Typ).toInt();
    if (TypID==0)
    {
        ui->comboBoxBeGroEin->setEnabled(false);
        ui->spinBoxBeNumEin->setEnabled(false);
        return;
    }
    GroessenTabelle *Groessen=Daten->getGroessen(&TypID,1);
    for (int i=0;i<Groessen->Anzahl;++i)
    {
        ui->comboBoxBeGroEin->addItem(Groessen->Namen[i],QVariant(Groessen->IDs[i]));
    }
    delete Groessen;
    ui->comboBoxBeGroEin->addItem(QString::fromUtf8("Unbekante Größe"),QVariant(0));
    std::cout<<Daten->freieNummer(TypID);
    int An,End;
    Daten->getNummerBereich(TypID,&An,&End);
    ui->spinBoxBeNumEin->setMaximum(End);
    ui->spinBoxBeNumEin->setMinimum(An);
    ui->spinBoxBeNumEin->setValue(Daten->freieNummer(TypID));
    ui->comboBoxBeGroEin->setEnabled(true);
    ui->spinBoxBeNumEin->setEnabled(true);
}

void MainWindow::LineEditSuchNameChange(QString SuchFilter)
{
    PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex(),SuchFilter);
}

/*!
 * \brief MainWindow::NamenContextMenuEvent Zeigt das Kontentmenü für die Personenliste an. Es bietet die Möglichkeit zum Löschen einer Person.
 * \param Pos Position an der dan Menü erscheinen soll.
 */
void MainWindow::NamenContextMenuEvent(const QPoint &Pos)
{
    QMenu menu(this);
    //menu.addAction("Bearbeiten");
    menu.addAction(ActionPersonLoeschen);
    menu.exec(ui->tablePersonen->viewport()->mapToGlobal(Pos));
}

void MainWindow::PerKleidungslistefuellen(int FilterTyp)
{
    int Filter=ui->comboBox_eigenFilter->itemData(FilterTyp).toInt();
    PerKleider->setFilterTyp(Filter);
}

/*!
 * \brief MainWindow::PersonenAnzeigen füllt die Listview mit Daten der Pesonen
 * \param JFFilter Ausgewählte Zeile der Combobox
 * \param NamenFilter Eingegebener Suchbegriff
 */
void MainWindow::PersonenAnzeigen(int JFFilter,QString NamenFilter)
{
    Personen.clear();
    QStringList Zeile;
    Zeile.append("ID");
    Zeile.append("Vorname");
    Zeile.append("Nachname");
    Zeile.append("Jugendfeuerwehr");
    Personen.setHorizontalHeaderLabels(Zeile);
    int FilterNr=0,FilterAns=0;
    if (JFFilter>0)
    {
        FilterAns=1;
        FilterNr=ui->comboBoxPerJFFilter->itemData(JFFilter).toInt();
        std::cout<<FilterNr<<std::endl;
    }
    PersonenTabelle *PerDaten=NULL;
    if (NamenFilter.isEmpty())
        PerDaten=Daten->getPersonen(&FilterNr,FilterAns);
    else
        PerDaten=Daten->getPersonen(&FilterNr,FilterAns,NamenFilter);
    for (int i=0;i<PerDaten->Anzahl;++i)
    {
        QList<QStandardItem*> Zeile;
        Zeile.append(new QStandardItem(QString::number(PerDaten->ID[i])));
        Zeile.append(new QStandardItem(PerDaten->Vorname[i]));
        Zeile.append(new QStandardItem(PerDaten->Nachname[i]));
        Zeile.append(new QStandardItem(PerDaten->JugendFeuerwehr[i]));
        Personen.appendRow(Zeile);
    }
    delete PerDaten;
}

void MainWindow::PersonAusleih(int ID)
{
    QString VName,NName,Gruppe,Text="<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">%1 %2 - %3</span></p></body></html>";
    int GID;
    if (Daten->getPersonenInfo(ID,&VName,&NName,&Gruppe,&GID))
    {
        PersonenID=ID;
        ui->label_Name->setText(Text.arg(VName,NName,Gruppe));
        ui->tab_einKleiden->setEnabled(true);
        PerKleider->setFilterPerson(PersonenID);
        ui->comboBox_AusTypFilter->setCurrentIndex(0);
        AusTypFiltergeaendert(0);
    }
}

void MainWindow::PersonAusgewaehlt(const QModelIndex &neu, const QModelIndex )
{

    QModelIndex Index=ProPersonen.mapToSource(ProPersonen.index(neu.row(),0));
    int row=Index.row();
    int ID=Personen.data(Personen.index(row,0)).toInt();
    PersonAusleih(ID);
    return;
}

void MainWindow::PersonHinClicked()
{
    QString Vorname= ui->linePerVor->text();
    QString Nachname=ui->lineEditPerNach->text();
    int JF=ui->comboPerJFEin->itemData(ui->comboPerJFEin->currentIndex()).toInt();
    if (Vorname.isEmpty())
    {
        QMessageBox::warning(this,"Vorname Fehlet!",QString::fromUtf8("Bitte Geben sie den Vornamen der Person ein, die hinzugefügt werden soll"));
        return;
    }
    if (Nachname.isEmpty())
    {
        QMessageBox::warning(this,"Nachname Fehlet!",QString::fromUtf8("Bitte Geben sie den Nachnamen der Person ein, die hinzugefügt werden soll"));
        return;
    }
    Daten->addPerson(Nachname,Vorname,JF);
    PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex(),ui->lineEditSuchName->text());
    PersonHinCancel();
}

void MainWindow::PersonHinCancel()
{
    ui->lineEditPerNach->clear();
    ui->linePerVor->clear();
}

/*!
 * \brief De Slot MainWindow::PersonListeDoubleClicked Wächselt die aktive Ansicht des Fenstes.
 * \param Index
 */
void MainWindow::PersonListeDoubleClicked(const QModelIndex &)
{
    ui->tabWidget->setCurrentIndex(2);
}

/*!
 * \brief MainWindow::PersonLoeschen Für den Löschvorgang der markierten Person aus. Und Prüft, ob sie noch Kleidungsstücke ausgelienen hat.
 */
void MainWindow::PersonLoeschen()
{
    //Holen des Orginal Indexes.
    QModelIndex Index=ProPersonen.mapToSource(ProPersonen.index(ui->tablePersonen->currentIndex().row(),0));
    int id=Personen.data(Index).toInt();
    KleiderTabelle *Kleider=Daten->getKleidervonPerson(id,0);
    if (Kleider->Anzahl==0)
    {
        if (QMessageBox::information(this,QString::fromUtf8("Person löschen"),QString::fromUtf8("Sind Sie sicher, dass sie ausgewälte die Person löschen wollen?"),
                                      QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        Daten->removePerson(id);
        PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex(),ui->lineEditSuchName->text());
    }
    else
    {
        QMessageBox::warning(this,QString::fromUtf8("Die Person hat noch Kleidungsstücke."),QString::fromUtf8("Die Person kann nicht gelöscht werden, da sie noch Kleidungsstücke ausgeliehen hat."),QMessageBox::Ok);
    }
    delete Kleider;
}

void MainWindow::RadiobuttomCilcked()
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


void MainWindow::ZeigeInfo()
{
    QString UberText;
    UberText=QString::fromUtf8("<html><head></head><body><h1>Kleiderkammer</h1><p>Version: %1</p><p>Diese Anwendung dient der Verwaltung einer Kleiderkammer.</p><p>Kleiderkammer ist Freie Software: Sie können es unter den Bedingungen der GNU Affero General Public License, wie von der Free Software Foundation,Version 3 der Lizenz, weiterverbreiten und/oder modifizieren.</p><p>Kleiderkammer wird in der Hoffnung, dass es nützlich sein wird, aber OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK. Siehe die GNU Affero General Public License für weitere Details.</p><p>Sie sollten eine Kopie der GNU Affero General Public License zusammen mit diesem Programm erhalten haben. Wenn nicht, siehe <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.</p></bod></html>");
    UberText=UberText.arg(VER_NUMBER_STRING);
    QMessageBox::about(this,QString::fromUtf8("Über Kleiderkammer"),UberText);
}

void MainWindow::ZeigePersonKleider(int ID)
{
    ui->tabWidget->setCurrentIndex(2);
    PersonAusleih(ID);
}

void MainWindow::ZeigeQTInfo()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::Zurueckgeben()
{
    QModelIndex Index=ProPerKleider.mapToSource(ProPerKleider.index(ui->tableView_KleiPerson->currentIndex().row(),0));
    int id=PerKleider->getKleidungsId(Index.row());
    if (id==0)
        return;
    Daten->rueckgabeKleidungsstueck(id);
    KleiderAus->update();
    PerKleidungslistefuellen(ui->comboBox_eigenFilter->currentIndex());
    KleidunginKammerAnzeigen(ui->comboBoxBekFilter->currentIndex());
}