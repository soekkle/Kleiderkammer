#include "widgeteinkleidentab.h"
#include "ui_widgeteinkleidentab.h"

WidgetEinkleidenTab::WidgetEinkleidenTab(DatenQuelle *Daten,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WidgetEinkleidenTab)
{
    ui->setupUi(this);
    this->Daten=Daten;
    KleiderAus = new KleidungsTableview(Daten,0,this);
    PerKleider = new KleidungsTableview(Daten,1,this);

    ComboBox=new ComboboxGroessenDelegate(Daten,this);

    ProKleiderAus.setSourceModel(KleiderAus);
    ui->table_Kleileihen->setModel(&ProKleiderAus);
    ui->table_Kleileihen->setItemDelegateForColumn(2,ComboBox);

    ProPerKleider.setSourceModel(PerKleider);
    ui->tableView_KleiPerson->setModel(&ProPerKleider);
    ui->tableView_KleiPerson->setItemDelegateForColumn(2,ComboBox);

    connect(ui->comboBox_AusTypFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(AusTypFiltergeaendert(int)));
    connect(ui->comboBox_AusGroFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(AusGroessenFiltergeaendert(int)));
    connect(ui->lineEdit_AusNum,SIGNAL(textChanged(QString)),this,SLOT(LineEditAusNummerChange(QString)));
    connect(ui->pushButton_leihen,SIGNAL(clicked()),this,SLOT(Auslehenclicked()));
    connect(ui->comboBox_eigenFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(PerKleidungslistefuellen(int)));
    connect(ui->pushButton_zuruck,SIGNAL(clicked()),this,SLOT(Zurueckgeben()));

}

WidgetEinkleidenTab::~WidgetEinkleidenTab()
{
    delete ComboBox;
    delete ui;
    delete KleiderAus;
    delete PerKleider;
}

void WidgetEinkleidenTab::AusGroessenFiltergeaendert(int Groesse)
{
    int GroFilter;
    GroFilter=ui->comboBox_AusGroFilter->itemData(Groesse).toInt();
    KleiderAus->setFilterGroesse(GroFilter);
}

void WidgetEinkleidenTab::Auslehenclicked()
{
    if (!ui->table_Kleileihen->selectionModel()->hasSelection())
    {
        QMessageBox::information(this,"Information",QString::fromUtf8("Sie müssen ein Kleidungsteil Auswählen"));
        return;
    }
    QModelIndex Index=ProKleiderAus.mapToSource(ProKleiderAus.index(ui->table_Kleileihen->currentIndex().row(),0));
    int id=KleiderAus->getKleidungsId(Index.row());
    if (id==0)
        return;
    int Nummer,Typ,Groesse;
    if (!Daten->getKleidungsInfoByID(id,&Nummer,&Typ,&Groesse))
        return;
    KleiderTabelle *Bestand=Daten->getKleidervonPerson(PersonenID,Typ);
    if (Bestand->Anzahl==1)
    {
        int ruck=QMessageBox::question(this,QString::fromUtf8("Rückgebe"),QString::fromUtf8("Wurde das Kleidungsstück mit der Nummer %1 zurückgegeben?").arg(Bestand->Nummer[0]),
                QMessageBox::Yes|QMessageBox::No);
        if (ruck==QMessageBox::Yes)
        {
            Daten->rueckgabeKleidungsstueck(Bestand->ID[0]);
        }
    }
    delete Bestand;
    Daten->KleidungsstueckzuordnenbyID(id,PersonenID);
    //Ausleihlistefuellen
    KleiderAus->setFilterTyp(ui->comboBox_AusTypFilter->itemData(ui->comboBox_AusTypFilter->currentIndex()).toInt());
    KleiderAus->setFilterGroesse(ui->comboBox_AusGroFilter->itemData(ui->comboBox_AusGroFilter->currentIndex()).toInt());
    PerKleidungslistefuellen(ui->comboBox_eigenFilter->currentIndex());
}

void WidgetEinkleidenTab::AusTypFiltergeaendert(int Typ)
{
    ui->comboBox_AusGroFilter->clear();
    ui->lineEdit_AusNum->clear();
    if (Typ==0)
    {
        ui->comboBox_AusGroFilter->setEnabled(false);
        KleiderAus->setFilterTyp(0);
        return;
    }
    ui->comboBox_AusGroFilter->addItem("Alle",QVariant(0));
    int Gro=ui->comboBox_AusTypFilter->itemData(Typ).toInt();
    KleiderAus->setFilterTyp(Gro);
    GroessenTabelle *Groessen=Daten->getGroessen(&Gro,1);
    for (int i=0;i<Groessen->Anzahl;++i)
    {
        ui->comboBox_AusGroFilter->addItem(Groessen->Namen[i],QVariant(Groessen->IDs[i]));
    }
    delete Groessen;
    ui->comboBox_AusGroFilter->setEnabled(true);
}

void WidgetEinkleidenTab::DatenGeaendert()
{
    ui->comboBox_AusGroFilter->clear();
    ui->comboBox_eigenFilter->clear();
    ui->comboBox_AusTypFilter->clear();
    ui->comboBox_AusGroFilter->addItem("Alle",QVariant(0));
    ui->comboBox_eigenFilter->addItem("Alle",QVariant(0));
    ui->comboBox_AusTypFilter->addItem("Alle",QVariant(0));
    /* Fült alle komboboxen die die Typen der Kleidungsstücke benötigen.
       Ebenfalls werden die ID als Daten dazu gespeichert.*/
    Kleidungstypentabelle *KleiTyp=Daten->getKleidungstypen();
    for (int i=0;i<KleiTyp->Anzahl;++i)
    {
        ui->comboBox_AusTypFilter->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
        ui->comboBox_eigenFilter->addItem(KleiTyp->Name[i],QVariant(KleiTyp->ID[i]));
    }
    delete KleiTyp;
}

void WidgetEinkleidenTab::LineEditAusNummerChange(QString Nummer)
{
    KleiderAus->setFilterNummer(Nummer);
}

void WidgetEinkleidenTab::PerKleidungslistefuellen(int FilterTyp)
{
    int Filter=ui->comboBox_eigenFilter->itemData(FilterTyp).toInt();
    PerKleider->setFilterTyp(Filter);
}

void WidgetEinkleidenTab::showPerson(int ID)
{
    QString VName,NName,Gruppe,Text="<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">%1 %2 - %3</span></p></body></html>";
    int GID;
    if (Daten->getPersonenInfo(ID,&VName,&NName,&Gruppe,&GID))
    {
        PersonenID=ID;
        ui->label_Name->setText(Text.arg(VName,NName,Gruppe));
        this->setEnabled(true);
        PerKleider->setFilterPerson(PersonenID);
        ui->comboBox_AusTypFilter->setCurrentIndex(0);
        AusTypFiltergeaendert(0);
    }
}

void WidgetEinkleidenTab::Zurueckgeben()
{
    if (!ui->tableView_KleiPerson->selectionModel()->hasSelection())
    {
        QMessageBox::information(this,"Information",QString::fromUtf8("Sie müssen ein Kleidungsteil Auswählen"));
        return;
    }
    QModelIndex Index=ProPerKleider.mapToSource(ProPerKleider.index(ui->tableView_KleiPerson->currentIndex().row(),0));
    int id=PerKleider->getKleidungsId(Index.row());
    if (id==0)
        return;
    Daten->rueckgabeKleidungsstueck(id);
    KleiderAus->update();
    PerKleidungslistefuellen(ui->comboBox_eigenFilter->currentIndex());
    KleidungVerschoben();
}

