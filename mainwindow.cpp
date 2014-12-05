#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);   
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
    ui->tableKleidung->setModel(&Kleidungstuecke);
    ui->tablePersonen->setModel(&Personen);
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
    QStringList Zeile;
    Zeile.append("Nummer");
    Zeile.append("Typ");
    Zeile.append(QString::fromLocal8Bit("Größe"));
    Kleidungstuecke.setHorizontalHeaderLabels(Zeile);
    connect(ui->actionKleidungstypen_verwalten,SIGNAL(triggered()),Typen,SLOT(exec()));
    connect(ui->comboBoxPerJFFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(PersonenAnzeigen(int)));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(PersonHinClicked()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(PersonHinCancel()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Daten;
}


void MainWindow::PersonenAnzeigen(int Filter)
{
    Personen.clear();
    QStringList Zeile;
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
        Zeile.append(new QStandardItem(PerDaten->Vorname[i]));
        Zeile.append(new QStandardItem(PerDaten->Nachname[i]));
        Zeile.append(new QStandardItem(PerDaten->JugendFeuerwehr[i]));
        Personen.appendRow(Zeile);
    }
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
    PersonHinCancel();
}

void MainWindow::PersonHinCancel()
{
    ui->lineEditPerNach->clear();
    ui->linePerVor->clear();
}
