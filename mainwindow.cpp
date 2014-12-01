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
    connect(ui->actionKleidungstypen_verwalten,SIGNAL(triggered()),Typen,SLOT(exec()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Daten;
}

