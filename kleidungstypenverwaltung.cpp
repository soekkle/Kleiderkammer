#include "kleidungstypenverwaltung.h"
#include "ui_kleidungstypenverwaltung.h"

KleidungsTypenVerwaltung::KleidungsTypenVerwaltung(DatenQuelle *Quelle,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KleidungsTypenVerwaltung)
{
    ui->setupUi(this);
    Daten=Quelle;
    Typen=new QStandardItemModel(this);
    Groessen=new KleidungsGroessenTableView(Daten,this);
    ui->table_Typ->setModel(Typen);
    ui->table_Groesse->setModel(Groessen);
    ui->table_Groesse->setItemDelegateForColumn(1,&SpinBox);
    QStringList Header;
    Header.append("ID");
    Header.append("Name");
    Header.append("Anfang");
    Header.append("Ende");
    Typen->setHorizontalHeaderLabels(Header);
    ui->table_Typ->setColumnHidden(0,true);
    connect(ui->Button_Typ,SIGNAL(clicked()),this,SLOT(Typanlegen()));
    connect(ui->Button_Groesse,SIGNAL(clicked()),this,SLOT(GroesseAnlegen()));
    connect(ui->table_Typ->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(Typwahlen(QItemSelection,QItemSelection)));
}

KleidungsTypenVerwaltung::~KleidungsTypenVerwaltung()
{
    delete Typen;
    delete Groessen;
    delete ui;
}

int KleidungsTypenVerwaltung::exec()
{
    Typentable();
    return QDialog::exec();
}

void KleidungsTypenVerwaltung::GroesseAnlegen()
{
    QString Name=ui->edit_Groesse->text();
    int Rang=ui->spin_Gros_Rang->value();
    if(!Name.isEmpty())
    {
        int id=Daten->addGroesse(Name,GroessenFilter);
        Daten->setRangGroesse(id,Rang);

    }
    Groessen->update();
    datenGeaendert();
}

void KleidungsTypenVerwaltung::Typanlegen()
{
    QString Name=ui->edit_Typ_Name->text();
    int Anfang=-1,Ende=-1;
    Anfang=ui->spin_Typ_An->value();
    Ende=ui->spin_Typ_En->value();
    if (!Name.isEmpty())
    {
        GroessenFilter=Daten->addKleidungstyp(Name,Anfang,Ende);
    }
    Typentable();
    datenGeaendert();
}

void KleidungsTypenVerwaltung::Typentable()
{
    Typen->clear();
    Kleidungstypentabelle *TypenTab=Daten->getKleidungstypen();
    QStringList Header;
    Header.append("ID");
    Header.append("Name");
    Header.append("Anfang");
    Header.append("Ende");
    Typen->setHorizontalHeaderLabels(Header);
    ui->table_Typ->setColumnHidden(0,true);

    for (int i=0;i<TypenTab->Anzahl;++i)
    {
        QList<QStandardItem*> Zeile;
        Zeile.append(new QStandardItem(QString::number(TypenTab->ID[i])));
        Zeile.append(new QStandardItem(TypenTab->Name[i]));
        Zeile.append(new QStandardItem(QString::number(TypenTab->AnfangsNummer[i])));
        Zeile.append(new QStandardItem(QString::number(TypenTab->EndNummer[i])));
        Typen->appendRow(Zeile);
    }
    delete TypenTab;
}

void KleidungsTypenVerwaltung::Typwahlen(const QItemSelection &neu, const QItemSelection &)
{
    if (neu.isEmpty())
    {
        Groessen->clear();
        ui->groupBox_2->setEnabled(false);
        return;
    }
    ui->groupBox_2->setEnabled(true);
    ui->groupBox_4->setEnabled(true);
    GroessenFilter=Typen->item(neu.indexes().first().row(),0)->text().toInt();
    Groessen->setKleidungsTyp(GroessenFilter);
}
