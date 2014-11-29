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
    /*std::cout<<Daten->addJugendfeuerwehr("Entenhausen")<<std::endl;
    std::cout<<Daten->addPerson("Mustermann","Max",1)<<std::endl;
    std::cout<<Daten->addKleidungstyp("Arbeitshose",1001,2000)<<std::endl;
    std::cout<<Daten->addGroesse("S",1)<<std::endl;
    std::cout<<Daten->addGroesse("M",1)<<std::endl;
    std::cout<<Daten->addGroesse("L",1)<<std::endl;
    std::cout<<Daten->addKleidungstyp("Jacke",2001,3000)<<std::endl;
    std::cout<<Daten->addGroesse("S",2)<<std::endl;
    std::cout<<Daten->addGroesse("M",2)<<std::endl;
    std::cout<<Daten->addGroesse("L",2)<<std::endl;
    std::cout<<Daten->addKleiderstueck(1,1)<<std::endl;
    std::cout<<Daten->addKleiderstueck(1,2)<<std::endl;
    std::cout<<Daten->addKleiderstueck(1,3)<<std::endl;
    std::cout<<Daten->addKleiderstueck(2,1)<<std::endl;
    std::cout<<Daten->addKleiderstueck(2,2)<<std::endl;
    std::cout<<Daten->addKleiderstueck(2,3)<<std::endl;
    std::cout<<Daten->KleidungsstueckzuordnenbyID(1,1);*/
    Kleidungstypentabelle *DatenTyp=Daten->getKleidungstypen();
    QStandardItemModel *DatenView=new QStandardItemModel(DatenTyp->Anzahl,4,this);
    ui->tableView->setModel(DatenView);
    DatenView->setHeaderData(0,Qt::Horizontal,tr("ID"));
    DatenView->setHeaderData(1,Qt::Horizontal,QString("Name"));
    DatenView->setHeaderData(2,Qt::Horizontal,tr("Anfangsnummer"));
    DatenView->setHeaderData(3,Qt::Horizontal,tr("Endnummer"));
    for(int i=0;i<DatenTyp->Anzahl;++i)
    {
        QStandardItem *item=new QStandardItem(QString::number(DatenTyp->ID[i]));
        DatenView->setItem(i,0,item);
        item=new QStandardItem(DatenTyp->Name[i]);
        DatenView->setItem(i,1,item);
        item=new QStandardItem(QString::number(DatenTyp->AnfangsNummer[i]));
        DatenView->setItem(i,2,item);
        item=new QStandardItem(QString::number(DatenTyp->EndNummer[i]));
        DatenView->setItem(i,3,item);
        //std::cout<<DatenGro->IDs[i]<<" : "<<DatenGro->Namen.at(i).toStdString()<<" : "<<DatenGro->Typ[i].toStdString()<<std::endl;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Daten;
}
