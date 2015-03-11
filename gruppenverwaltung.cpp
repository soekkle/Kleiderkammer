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
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(GruppenContextMenu(QPoint)));
    // Erzeugen und Inizialisiern der QAction Objekte für das Menü.
    ActionGruppeLoeschen=new QAction(QString::fromUtf8("Löschen"),this);
    ActionGruppeLoeschen->setToolTip(QString::fromUtf8("Löscht die Ausgewälte Gruppe"));
    connect(ActionGruppeLoeschen,SIGNAL(triggered()),this,SLOT(GruppeLoeschen()));
}

Gruppenverwaltung::~Gruppenverwaltung()
{
    delete ActionGruppeLoeschen;
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
/*!
 * \brief Gruppenverwaltung::GruppenContextMenu Zeigt das Contextmenü im Dialog an.
 * \param Pos Position an der das Contextmenü erscheint.
 */
void Gruppenverwaltung::GruppenContextMenu(QPoint Pos)
{
    QMenu menu(this);
    menu.addAction(ActionGruppeLoeschen);
    menu.exec(ui->tableView->viewport()->mapToGlobal(Pos));
}

/*!
 * \brief Gruppenverwaltung::GruppeLoeschen löst das Löschen der Ausgewählten Gruppe aus.
 */
void Gruppenverwaltung::GruppeLoeschen()
{
    if (QMessageBox::information(this,QString::fromUtf8("Gruppe Löschen"),QString::fromUtf8("Sind Sie sicher Dass sie ausgewälte die Gruppe Löschen wollen?"),
                                 QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        return;
    int id=Gruppen.data(Gruppen.index(ui->tableView->currentIndex().row(),0)).toInt();
    Daten->removeJugendferweher(id);
    gruppenTabelleFullen();
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
