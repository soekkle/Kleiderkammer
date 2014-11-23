#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Daten=new SQLiteQuelle("Test.db");
    std::cout<<Daten->addJugendfeuerwehr("Entenhausen")<<std::endl;
    std::cout<<Daten->addPerson("Mustermann","Max",1)<<std::endl;
    std::cout<<Daten->addKleidungstyp("Arbeitshose",1001,2000)<<std::endl;
    std::cout<<Daten->addGroesse("XXS",1)<<std::endl;
    std::cout<<Daten->addKleiderstueck(1,1)<<std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Daten;
}
