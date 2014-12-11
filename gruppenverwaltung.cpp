#include "gruppenverwaltung.h"
#include "ui_gruppenverwaltung.h"

Gruppenverwaltung::Gruppenverwaltung(DatenQuelle *Daten, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gruppenverwaltung)
{
    ui->setupUi(this);
    this->Daten=Daten;
    ui->tableView->setModel(&Gruppen);
    gruppenTabelleFullen();
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(gruppenHin()));
}

Gruppenverwaltung::~Gruppenverwaltung()
{
    delete ui;
}

void Gruppenverwaltung::gruppenHin()
{
    QString Name=ui->lineEdit->text();
    if (Name.isEmpty())
        return;
    Daten->addJugendfeuerwehr(Name);
    ui->lineEdit->clear();
    gruppenTabelleFullen();
    datenGeaendert();
}

void Gruppenverwaltung::gruppenTabelleFullen()
{
    Gruppen.clear();
    QStringList Kopf;
    Kopf.append("ID");
    Kopf.append("Jugendfeuerwehr");
    Gruppen.setHorizontalHeaderLabels(Kopf);
    JugendFeuerwehrTabelle *GruppenTab=Daten->getJugendfeuerwehr();
    for (int i=0;i<GruppenTab->Anzahl;++i)
    {
         QList<QStandardItem*> Zeile;
         Zeile.append(new QStandardItem(QString::number(GruppenTab->ID[i])));
         Zeile.append(new QStandardItem(GruppenTab->Name[i]));
         Gruppen.appendRow(Zeile);
    }
    delete GruppenTab;
}
