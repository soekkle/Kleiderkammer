#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PersonenID=0;
#if __linux__ || __unix__
    QString Ort=QDir::homePath();
    Ort=Ort.append("/.config/Kleiderkammer");
#elif __WIN32__ || _MSC_VER
    QString Ort="$APPDATA$/Kleiderkammer";
#endif
    {
        QDir Appdir(Ort);
        if(!Appdir.exists())
        {
            Appdir.mkpath(Ort);
        }
    }
    Ort  =Ort.append("/Daten.sqlite");
    std::clog <<Ort.toStdString()<<std::endl;
    Daten=new SQLiteQuelle(Ort);
    Typen=new KleidungsTypenVerwaltung(Daten,this);
    Gruppen=new Gruppenverwaltung(Daten,this);
    ui->tableKleidung->setModel(&Kleidungstuecke);
    ui->tablePersonen->setModel(&Personen);
    ui->table_Kleileihen->setModel(&KleiderAus);
    ComboboxFuellen();
    connect(ui->actionKleidungstypen_verwalten,SIGNAL(triggered()),Typen,SLOT(exec()));
    connect(Typen,SIGNAL(datenGeaendert()),this,SLOT(ComboboxFuellen()));
    connect(ui->actionGruppen_Verwalten,SIGNAL(triggered()),Gruppen,SLOT(exec()));
    connect(Gruppen,SIGNAL(datenGeaendert()),this,SLOT(ComboboxFuellen()));
    connect(ui->comboBoxPerJFFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(PersonenAnzeigen(int)));
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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Daten;
}

void MainWindow::AusGroessenFiltergeaendert(int Groesse)
{
    int GroFilter,TypFilter;
    TypFilter=ui->comboBox_AusTypFilter->itemData(ui->comboBox_AusTypFilter->currentIndex()).toInt();
    GroFilter=ui->comboBox_AusGroFilter->itemData(Groesse).toInt();
    Ausleihlistefuellen(TypFilter,GroFilter);
}

void MainWindow::Auslehenclicked()
{
    int id=KleiderAus.data(KleiderAus.index(ui->table_Kleileihen->currentIndex().row(),0)).toInt();
    if (id==0)
        return;
    std::clog<<PersonenID<<" : "<<id<<std::endl;
    Daten->KleidungsstueckzuordnenbyID(id,PersonenID);
    Ausleihlistefuellen(ui->comboBox_AusTypFilter->itemData(ui->comboBox_AusTypFilter->currentIndex()).toInt(),
                        ui->comboBox_AusGroFilter->itemData(ui->comboBox_AusGroFilter->currentIndex()).toInt());
}

void MainWindow::Ausleihlistefuellen(int Filtertyp, int FilterGroesse)
{
    KleiderAus.clear();
    QStringList Titel;
    Titel.append("ID");
    Titel.append("Typ");
    Titel.append(QString::fromLocal8Bit("Größe"));
    Titel.append("Nummer");
    KleiderAus.setHorizontalHeaderLabels(Titel);
    KleiderTabelle *Kleidung=Daten->getKleiderinKammer(Filtertyp,FilterGroesse);
    for (int i=0;i<Kleidung->Anzahl;++i)
    {
        QList<QStandardItem*> Zeile;
        Zeile.append(new QStandardItem(QString::number(Kleidung->ID[i])));
        Zeile.append(new QStandardItem(Kleidung->Typ[i]));
        Zeile.append(new QStandardItem(Kleidung->Groesse[i]));
        Zeile.append(new QStandardItem(QString::number(Kleidung->Nummer[i])));
        KleiderAus.appendRow(Zeile);
    }
    delete Kleidung;
    ui->table_Kleileihen->hideColumn(0);
}

void MainWindow::AusTypFiltergeaendert(int Typ)
{
    ui->comboBox_AusGroFilter->clear();
    if (Typ==0)
    {
        ui->comboBox_AusGroFilter->setEnabled(false);
        return;
    }
    ui->comboBox_AusGroFilter->addItem("Alle",QVariant(0));
    int Gro=ui->comboBox_AusTypFilter->itemData(Typ).toInt();
    GroessenTabelle *Groessen=Daten->getGroessen(&Gro,1);
    for (int i=0;i<Groessen->Anzahl;++i)
    {
        ui->comboBox_AusGroFilter->addItem(Groessen->Namen[i],QVariant(Groessen->IDs[i]));
    }
    delete Groessen;
    ui->comboBox_AusGroFilter->setEnabled(true);
}

void MainWindow::ComboboxFuellen()
{
    JugendFeuerwehrTabelle *JfDaten=Daten->getJugendfeuerwehr();
    ui->comboBoxPerJFFilter->clear();
    ui->comboBoxPerJFFilter->addItem("Alle",QVariant(0));
    ui->comboPerJFEin->clear();
    for (int i=0;i<JfDaten->Anzahl;++i)
    {
        ui->comboBoxPerJFFilter->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
        ui->comboPerJFEin->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
    }
    PersonenAnzeigen(0);
    delete JfDaten;
    ui->comboBoxBekFilter->clear();
    ui->comboBoxBeTypEin->clear();
    ui->comboBox_AusGroFilter->clear();
    ui->comboBox_eigenFilter->clear();
    ui->comboBox_AusTypFilter->clear();
    ui->comboBoxBekFilter->addItem("Alle",QVariant(0));
    ui->comboBox_AusGroFilter->addItem("Alle",QVariant(0));
    ui->comboBox_eigenFilter->addItem("Alle",QVariant(0));
    ui->comboBox_AusTypFilter->addItem("Alle",QVariant(0));
    ui->comboBoxBeTypEin->addItem(QString::fromLocal8Bit("Bitte Wählen"),QVariant(0));
    Kleidungstypentabelle *KleiTyp=Daten->getKleidungstypen();
    for (int i=0;i<KleiTyp->Anzahl;++i)
    {
        ui->comboBoxBekFilter->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
        ui->comboBoxBeTypEin->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
        ui->comboBox_AusTypFilter->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
        ui->comboBox_eigenFilter->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
    }
    KleidunginKammerAnzeigen(0);
    delete KleiTyp;
}

void MainWindow::KleidungHinCancel()
{
    ui->comboBoxBeTypEin->setCurrentIndex(0);
    ui->comboBoxBeGroEin->setCurrentIndex(0);
}

void MainWindow::KleidungHinClicked()
{
    if (ui->comboBoxBeTypEin->currentIndex()==0)
        return;
    int TypID,GroID,Nummer;
    TypID=ui->comboBoxBeTypEin->itemData(ui->comboBoxBeTypEin->currentIndex()).toInt();
    GroID=ui->comboBoxBeGroEin->itemData(ui->comboBoxBeGroEin->currentIndex()).toInt();
    Nummer=ui->spinBoxBeNumEin->value();
    std::cout<<TypID<<" : "<<GroID<<" : "<<Nummer<<std::endl;
    Daten->addKleiderstueck(TypID,GroID,Nummer);
    Kleidungstypgewaehlt(ui->comboBoxBekFilter->currentIndex());
    KleidungHinCancel();
}

void MainWindow::KleidunginKammerAnzeigen(int Filter)
{
    QStringList Zeile;
    Zeile.append("Nummer");
    Zeile.append("Typ");
    Zeile.append(QString::fromLocal8Bit("Größe"));
    Kleidungstuecke.clear();
    Kleidungstuecke.setHorizontalHeaderLabels(Zeile);
    int TypFilter=ui->comboBoxBekFilter->itemData(Filter).toInt();
    KleiderTabelle *Kleidung=Daten->getKleiderinKammer(TypFilter,-1);
    for (int i=0;i<Kleidung->Anzahl;++i)
    {
        QList<QStandardItem*> Zeile;
        Zeile.append(new QStandardItem(QString::number(Kleidung->Nummer[i])));
        Zeile.append(new QStandardItem(Kleidung->Typ[i]));
        Zeile.append(new QStandardItem(Kleidung->Groesse[i]));
        Kleidungstuecke.appendRow(Zeile);
    }
    delete Kleidung;
}

void MainWindow::Kleidungstypgewaehlt(int Typ)
{
    ui->comboBoxBeGroEin->clear();
    ui->comboBoxBeGroEin->addItem(QString::fromLocal8Bit("Bitte Wählen"),QVariant(0));
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
    std::cout<<Daten->freieNummer(TypID);
    int An,End;
    Daten->getNummerBereich(TypID,&An,&End);
    ui->spinBoxBeNumEin->setMaximum(End);
    ui->spinBoxBeNumEin->setMinimum(An);
    ui->spinBoxBeNumEin->setValue(Daten->freieNummer(TypID));
    ui->comboBoxBeGroEin->setEnabled(true);
    ui->spinBoxBeNumEin->setEnabled(true);
}

void MainWindow::PersonenAnzeigen(int Filter)
{
    Personen.clear();
    QStringList Zeile;
    Zeile.append("ID");
    Zeile.append("Vorname");
    Zeile.append("Nachname");
    Zeile.append("Jugendfeuerwehr");
    Personen.setHorizontalHeaderLabels(Zeile);
    int FilterNr=0,FilterAns=0;
    if (Filter>0)
    {
        FilterAns=1;
        FilterNr=ui->comboBoxPerJFFilter->itemData(Filter).toInt();
        std::cout<<FilterNr<<std::endl;
    }
    PersonenTabelle *PerDaten=Daten->getPersonen(&FilterNr,FilterAns);
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

    int row=neu.row();
    QString Name,Text="<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">%1</span></p></body></html>";
    Name=Personen.data(Personen.index(row,1)).toString();
    Name=Name.append(" ").append(Personen.data(Personen.index(row,2)).toString()).append(" - ");
    Name.append(Personen.data(Personen.index(row,3)).toString());
    ui->label_Name->setText(Text.arg(Name));
    ui->tab_einKleiden->setEnabled(true);
    ui->comboBox_AusTypFilter->setCurrentIndex(0);
    PersonenID=Personen.data(Personen.index(row,0)).toInt();
    return;
}

void MainWindow::PersonHinClicked()
{
    QString Vorname= ui->linePerVor->text();
    QString Nachname=ui->lineEditPerNach->text();
    int JF=ui->comboPerJFEin->itemData(ui->comboPerJFEin->currentIndex()).toInt();
    if (Vorname.isEmpty())
    {
        return;
    }
    if (Nachname.isEmpty())
    {
        return;
    }
    Daten->addPerson(Nachname,Vorname,JF);
    PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex());
    PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex());
    PersonHinCancel();
}

void MainWindow::PersonHinCancel()
{
    ui->lineEditPerNach->clear();
    ui->linePerVor->clear();
}
