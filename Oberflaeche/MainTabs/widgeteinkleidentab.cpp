/*
 * Copyright (C) 2016,2017 Sören Krecker
 *
 * This file is part of Kleiderkammer.
 *
 * Kleiderkammer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation version 3 of the License.
 *
 * Kleiderkammer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Kleiderkammer.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Diese Datei ist Teil von Kleiderkammer.
 *
 * Kleiderkammer ist Freie Software: Sie können es unter den Bedingungen
 * der GNU Affero General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz, weiterverbreiten und/oder modifizieren.
 *
 * Kleiderkammer wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN
 * ZWECK.Siehe die GNU Affero General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU Affero General Public License zusammen mit
 * diesem Programm erhalten haben. Wenn nicht, siehe
 * <http://www.gnu.org/licenses/>.
*/
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

    connect(ui->tableView_KleiPerson,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(TablePerKleiderContextMenuEvent(QPoint)));
    connect(ui->table_Kleileihen,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(TableKammerKJleiderContextMenuEvent(QPoint)));

    ActKleiderAus=new QAction(QString::fromUtf8("Ausleihen"),this);
    connect(ActKleiderAus,SIGNAL(triggered()),this,SLOT(Auslehenclicked()));
    ActKleiderRuck=new QAction(QString::fromUtf8("Zurückgeben"),this);
    connect(ActKleiderRuck,SIGNAL(triggered()),this,SLOT(Zurueckgeben()));
    ActLoeschenKleidungKammer=new QAction(QString::fromUtf8("Löschen"),this);
    connect(ActLoeschenKleidungKammer,SIGNAL(triggered()),this,SLOT(loeschenKleidungKammer()));
    ActLoeschenKleidungPerson=new QAction(QString::fromUtf8("Löschen"),this);
    connect(ActLoeschenKleidungPerson,SIGNAL(triggered()),this,SLOT(loeschenKleidungPerson()));
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

void WidgetEinkleidenTab::loeschenKleidungKammer()
{
    QModelIndex Index=ProKleiderAus.mapToSource(ProKleiderAus.index(ui->table_Kleileihen->currentIndex().row(),0));
    int id=KleiderAus->getKleidungsId(Index.row());
    if (id==0)
        return;
    if (QMessageBox::information(this,QString::fromUtf8("Kleidungsstück löschen"),QString::fromUtf8("Sind Sie sich sicher, dass sie das Ausgewälte Kleidungsstück löschen wollen?"),
                                 QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        Daten->removeKleidungsstueck(id);
    KleiderAus->setFilterTyp(ui->comboBox_AusTypFilter->itemData(ui->comboBox_AusTypFilter->currentIndex()).toInt());
    KleiderAus->setFilterGroesse(ui->comboBox_AusGroFilter->itemData(ui->comboBox_AusGroFilter->currentIndex()).toInt());
    PerKleidungslistefuellen(ui->comboBox_eigenFilter->currentIndex());
}

void WidgetEinkleidenTab::loeschenKleidungPerson()
{
    QModelIndex Index=ProPerKleider.mapToSource(ProPerKleider.index(ui->tableView_KleiPerson->currentIndex().row(),0));
    int id=PerKleider->getKleidungsId(Index.row());
    if (id==0)
        return;
    if (QMessageBox::information(this,QString::fromUtf8("Kleidungsstück löschen"),QString::fromUtf8("Sind Sie sich sicher, dass sie das Ausgewälte Kleidungsstück löschen wollen?"),
                                            QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                   Daten->removeKleidungsstueck(id);
    KleiderAus->update();
    PerKleidungslistefuellen(ui->comboBox_eigenFilter->currentIndex());
}

void WidgetEinkleidenTab::TableKammerKJleiderContextMenuEvent(const QPoint Pos)
{
    QMenu menu(this);
    menu.addAction(ActKleiderAus);
    menu.addAction(ActLoeschenKleidungKammer);
    menu.exec(ui->table_Kleileihen->viewport()->mapToGlobal(Pos));
}

void WidgetEinkleidenTab::TablePerKleiderContextMenuEvent(const QPoint Pos)
{
    QMenu menu(this);
    menu.addAction(ActKleiderRuck);
    menu.addAction(ActLoeschenKleidungPerson);
    menu.exec(ui->tableView_KleiPerson->viewport()->mapToGlobal(Pos));
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

