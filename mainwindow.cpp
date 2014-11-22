#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Daten=new SQLiteQuelle("Test.db");
    Daten->addPerson("Mustermann","Max",1);
    Daten->addKleiderstueck(1,"XS",500);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Daten;
}
