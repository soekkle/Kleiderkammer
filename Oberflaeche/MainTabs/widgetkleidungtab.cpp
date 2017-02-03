#include "widgetkleidungtab.h"
#include "ui_widgetkleidungtab.h"

WidgetKleidungTab::WidgetKleidungTab(DatenQuelle *Daten, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WidgetKleidungTab)
{
    ui->setupUi(this);
    this->Daten=Daten;
    ComboBox=new ComboboxGroessenDelegate(Daten,this);

    Kleidungstuecke = new KleidungsTableview(Daten,0,this);// Erzeugen des DatenModells
    Kleidungstuecke->setDateTitel(QString::fromUtf8("Zurückgegeben"));
    ProKleidungstuecke.setSourceModel(Kleidungstuecke);//Verbinden Des Proxiemodell mit dem Datenmodell.
    ui->tableKleidung->setModel(&ProKleidungstuecke);//Setzt die Modelle zur Anzeige der Daten.
    ui->tableKleidung->setItemDelegateForColumn(2,ComboBox);// Setzen der Combobox für die Größen

    // Herstellen der Verbindungen
    connect(ui->comboBoxBekFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(KleidunginKammerAnzeigen(int)));
    connect(ui->comboBoxBeTypEin,SIGNAL(currentIndexChanged(int)),this,SLOT(Kleidungstypgewaehlt(int)));
    connect(ui->buttonBox_2,SIGNAL(accepted()),this,SLOT(HinClicked()));
    connect(ui->buttonBox_2,SIGNAL(rejected()),this,SLOT(HinCancel()));
    connect(ui->tableKleidung,SIGNAL(customContextMenuRequested(const QPoint)),this, SLOT(TableContextMenuEvent(QPoint)));
    // Anlegen der Actions für das Kontexmenue
    ActionKleicungLoeschen=new QAction(QString::fromUtf8("Löschen"),this);
    ActionKleicungLoeschen->setToolTip(QString::fromUtf8("Löscht das Ausgewalte Kleidungsstück."));
    connect(ActionKleicungLoeschen,SIGNAL(triggered()),this,SLOT(KleidungLoeschen()));
    DatenGeaendert();
}

WidgetKleidungTab::~WidgetKleidungTab()
{
    delete ui;
    delete Kleidungstuecke;
    delete ComboBox;
}

void WidgetKleidungTab::DatenGeaendert()
{
    ui->comboBoxBekFilter->clear();
    ui->comboBoxBeTypEin->clear();
    ui->comboBoxBekFilter->addItem("Alle",QVariant(0));
    ui->comboBoxBeTypEin->addItem(QString::fromUtf8("Bitte Wählen"),QVariant(0));
    Kleidungstypentabelle *KleiTyp=Daten->getKleidungstypen();
    for (int i=0;i<KleiTyp->Anzahl;++i)
    {
        ui->comboBoxBekFilter->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
        ui->comboBoxBeTypEin->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
    }
    KleidunginKammerAnzeigen(0);
    delete KleiTyp;
}

void WidgetKleidungTab::HinCancel()
{
    ui->comboBoxBeTypEin->setCurrentIndex(0);
    ui->comboBoxBeGroEin->setCurrentIndex(0);
    KleidunginKammerAnzeigen(ui->comboBoxBekFilter->currentIndex());
}

void WidgetKleidungTab::HinClicked()
{
    if (ui->comboBoxBeTypEin->currentIndex()==0)
        return;
    if (ui->comboBoxBeGroEin->currentIndex()==0)
        return;
    int TypID,GroID,Nummer;
    TypID=ui->comboBoxBeTypEin->itemData(ui->comboBoxBeTypEin->currentIndex()).toInt();
    GroID=ui->comboBoxBeGroEin->itemData(ui->comboBoxBeGroEin->currentIndex()).toInt();
    Nummer=ui->spinBoxBeNumEin->value();
    if (Daten->addKleiderstueck(TypID,GroID,Nummer,false)==-1)
        QMessageBox::warning(this,QString::fromUtf8("Kleidungsstück schon vorhanden"),QString::fromUtf8("Das Kleidungsstück mit der Nummer %1 ist schon angelegt und kann deshalb nicht angelegt werden").arg(Nummer));
    Kleidungstypgewaehlt(ui->comboBoxBeTypEin->currentIndex());
    KleidunginKammerAnzeigen(ui->comboBoxBekFilter->currentIndex());
}

void WidgetKleidungTab::KleidunginKammerAnzeigen(int Filter)
{
    Kleidungstuecke->setFilterTyp(ui->comboBoxBekFilter->itemData(Filter).toInt());

}

/*!
 * \brief WidgetKleidungTab::KleidungLoeschen Löscht das ausgewälte Kleidungsstück aus der Kleiderkammer.
 */
void WidgetKleidungTab::KleidungLoeschen()
{
    //Holen des Orginal Indexes.
    QModelIndex Index=ProKleidungstuecke.mapToSource(ProKleidungstuecke.index(ui->tableKleidung->currentIndex().row(),0));
    int id=Kleidungstuecke->getKleidungsId(Index.row());
    if (QMessageBox::information(this,QString::fromUtf8("Kleidungsstück löschen"),QString::fromUtf8("Sind Sie sich sicher, dass sie das Ausgewälte Kleidungsstück löschen wollen?"),
                                 QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        Daten->removeKleidungsstueck(id);
    KleidunginKammerAnzeigen(ui->comboBoxBekFilter->currentIndex());
}

void WidgetKleidungTab::Kleidungstypgewaehlt(int Typ)
{
    ui->comboBoxBeGroEin->clear();
    ui->comboBoxBeGroEin->addItem(QString::fromUtf8("Bitte Wählen"),QVariant(-1));
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
    ui->comboBoxBeGroEin->addItem(QString::fromUtf8("Unbekante Größe"),QVariant(0));
    int An,End;
    Daten->getNummerBereich(TypID,&An,&End);
    ui->spinBoxBeNumEin->setMaximum(End);
    ui->spinBoxBeNumEin->setMinimum(An);
    ui->spinBoxBeNumEin->setValue(Daten->freieNummer(TypID));
    ui->comboBoxBeGroEin->setEnabled(true);
    ui->spinBoxBeNumEin->setEnabled(true);
}

/*!
 * \brief WidgetKleidungTab::TableContextMenuEvent Zeigt ein Kontextmenü für das Tableview TabelleKleider an.
 * \param Pos Position Auf der TableView, wo das Contextmenü angezeigt wird,
 */
void WidgetKleidungTab::TableContextMenuEvent(const QPoint Pos)
{
    QMenu menu(this);
    menu.addAction(ActionKleicungLoeschen);
    menu.exec(ui->tableKleidung->viewport()->mapToGlobal(Pos));
}

void WidgetKleidungTab::refrashTable()
{
    KleidunginKammerAnzeigen(ui->comboBoxBekFilter->currentIndex());
}
