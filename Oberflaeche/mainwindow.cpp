/*
 * Copyright (C) 2014-2016 Sören Krecker
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
    PersonBeabeiten=new PersonBearbeitenDialog(Daten,this);

    // Einsetzen der ausgelagreten Tabs
    KleidungTab=new WidgetKleidungTab(Daten,0);
    ui->tabWidget->addTab(KleidungTab,QString::fromUtf8("Kleidungsstücke"));

    EinkeleidenTab=new WidgetEinkeleidenTab(Daten,0);
    EinkeleidenTab->setEnabled(false);
    ui->tabWidget->addTab(EinkeleidenTab,"Einkleiden");

    BerichtTab=new WidgetBerichtTab(Daten,Ort,0);
    ui->tabWidget->addTab(BerichtTab,"Bericht");


    ProPersonen.setSourceModel(&Personen);
    ui->tablePersonen->setModel(&ProPersonen);
    ComboboxFuellen();//Inizalisiert alle Comboboxen.

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
    connect(ui->tablePersonen->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(PersonAusgewaehlt(QModelIndex,QModelIndex)));
    // Verbinden der Mainmenü einträgen
    connect(ui->actionBeenden,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionKleidungsst_ck,SIGNAL(triggered()),KleiderInfoSuchen,SLOT(exec()));
    connect(ui->action_ber,SIGNAL(triggered()),this,SLOT(ZeigeInfo()));
    connect(ui->action_ber_QT,SIGNAL(triggered()),this,SLOT(ZeigeQTInfo()));
    //Verboinden der ContextMenüs
    connect(ui->tablePersonen,SIGNAL(customContextMenuRequested(const QPoint)),this,SLOT(NamenContextMenuEvent(QPoint)));

    connect(ui->tablePersonen,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(PersonListeDoubleClicked(const QModelIndex &)));
    //Anlegen der Actionen
    ActionPersonBearbeiten=new QAction(QString::fromUtf8("Bearbeiten"),this);
    connect(ActionPersonBearbeiten,SIGNAL(triggered()),this,SLOT(PersonBearbeitenClicked()));
    ActionPersonLoeschen= new QAction(QString::fromUtf8("Löschen"),this);
    connect(ActionPersonLoeschen,SIGNAL(triggered()),this,SLOT(PersonLoeschen()));

    connect(EinkeleidenTab,SIGNAL(KleidungVerschoben()),KleidungTab,SLOT(refrashTable()));
#ifdef NOPRINT
    ui->pushButton_BeDr->close();
#endif
}

MainWindow::~MainWindow()
{
    delete ActionPersonLoeschen;
    delete ui;
    delete Daten;
    delete PersonBeabeiten;
    delete BerichtTab;
    delete EinkeleidenTab;
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
    for (int i=0;i<Datein.length()-9;++i)
    {
        QString BackAlt=DBBack;
        BackAlt.append(Datein[i]);
        QFile Alt(BackAlt);
        Alt.remove();
    }
    DBBack.append(QDateTime::currentDateTime().toString("yyMMddhhmmss")).append(".db");
    QFile::copy(DBAktuell,DBBack);
}

void MainWindow::ComboboxFuellen()
{
    /* Fült zwei Comboboxen mit den Namen der Eingetragenen Gruppen.
       und Speicher die id aus der Datenqelle dazu.*/
    JugendFeuerwehrTabelle *JfDaten=Daten->getJugendfeuerwehr();
    ui->comboBoxPerJFFilter->clear();
    ui->comboBoxPerJFFilter->addItem("Alle",QVariant(0));
    ui->comboPerJFEin->clear();
    for (int i=0;i<JfDaten->Anzahl;++i)
    {
        ui->comboBoxPerJFFilter->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
        ui->comboPerJFEin->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
    }
    PersonenAnzeigen(0,"");
    delete JfDaten;
    BerichtTab->DatenGeaendert();
    EinkeleidenTab->DatenGeaendert();
    KleidungTab->DatenGeaendert();
}


void MainWindow::ComboboxPerJFFilterGewahlt(int Pos)
{
    PersonenAnzeigen(Pos,ui->lineEditSuchName->text());
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
    menu.addAction(ActionPersonBearbeiten);
    menu.addAction(ActionPersonLoeschen);
    menu.exec(ui->tablePersonen->viewport()->mapToGlobal(Pos));
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

void MainWindow::PersonAusgewaehlt(const QModelIndex &neu, const QModelIndex )
{

    QModelIndex Index=ProPersonen.mapToSource(ProPersonen.index(neu.row(),0));
    int row=Index.row();
    int ID=Personen.data(Personen.index(row,0)).toInt();
    EinkeleidenTab->showPerson(ID);
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

/*!
 * \brief MainWindow::PersonBearbeitenClicked öffent ein Personbearbeiten Fester für die Ausgewälte Person
 */
void MainWindow::PersonBearbeitenClicked()
{
    QModelIndex Index=ProPersonen.mapToSource(ProPersonen.index(ui->tablePersonen->currentIndex().row(),0));
    int ID=Personen.data(Index).toInt();
    PersonBeabeiten->bearbeiten(ID);
    PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex(),ui->lineEditSuchName->text());
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
    EinkeleidenTab->showPerson(ID);
}

void MainWindow::ZeigeQTInfo()
{
    QMessageBox::aboutQt(this);
}
