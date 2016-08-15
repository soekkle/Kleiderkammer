#include "widgetpersonentab.h"
#include "ui_widgetpersonentab.h"

WidgetPersonenTab::WidgetPersonenTab(DatenQuelle *Daten, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WidgetPersonenTab)
{
    ui->setupUi(this);
    this->Daten=Daten;

    PersonBeabeiten=new PersonBearbeitenDialog(Daten,this);

    PersonenID=0;

    ProPersonen.setSourceModel(&Personen);
    ui->tablePersonen->setModel(&ProPersonen);

    connect(ui->comboBoxPerJFFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(ComboboxPerJFFilterGewahlt(int)));
    connect(ui->lineEditSuchName,SIGNAL(textChanged(QString)),this,SLOT(LineEditSuchNameChange(QString)));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(PersonHinClicked()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(PersonHinCancel()));
    connect(ui->tablePersonen->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(PersonAusgewaehlt(QModelIndex,QModelIndex)));
    //Verboinden der ContextMenüs
    connect(ui->tablePersonen,SIGNAL(customContextMenuRequested(const QPoint)),this,SLOT(NamenContextMenuEvent(QPoint)));
    connect(ui->tablePersonen,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(PersonListeDoubleClicked(const QModelIndex &)));
    //Anlegen der Actionen
    ActionPersonBearbeiten=new QAction(QString::fromUtf8("Bearbeiten"),this);
    connect(ActionPersonBearbeiten,SIGNAL(triggered()),this,SLOT(PersonBearbeitenClicked()));
    ActionPersonLoeschen= new QAction(QString::fromUtf8("Löschen"),this);
    connect(ActionPersonLoeschen,SIGNAL(triggered()),this,SLOT(PersonLoeschen()));

}

WidgetPersonenTab::~WidgetPersonenTab()
{
    delete ui;
    delete ActionPersonLoeschen;
    delete PersonBeabeiten;
}

void WidgetPersonenTab::DatenGeaendert()
{
    /* Fült zwei Comboboxen mit den Namen der Eingetragenen Gruppen.
       und Speicher die id aus der Datenqelle dazu.*/
    JugendFeuerwehrTabelle *JfDaten=Daten->getJugendfeuerwehr();
    ui->comboBoxPerJFFilter->clear();
    ui->comboBoxPerJFFilter->addItem("Alle",QVariant(0));
    ui->comboPerJFEin->clear();
    for (int i=0;i<JfDaten->Anzahl;++i)
    {
        ui->comboBoxPerJFFilter->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
        ui->comboPerJFEin->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
    }
    PersonenAnzeigen(0,"");
    delete JfDaten;
}

void WidgetPersonenTab::ComboboxPerJFFilterGewahlt(int Pos)
{
    PersonenAnzeigen(Pos,ui->lineEditSuchName->text());
}

void WidgetPersonenTab::LineEditSuchNameChange(QString SuchFilter)
{
    PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex(),SuchFilter);
}

/*!
 * \brief MainWindow::NamenContextMenuEvent Zeigt das Kontentmenü für die Personenliste an. Es bietet die Möglichkeit zum Löschen einer Person.
 * \param Pos Position an der dan Menü erscheinen soll.
 */
void WidgetPersonenTab::NamenContextMenuEvent(const QPoint &Pos)
{
    QMenu menu(this);
    menu.addAction(ActionPersonBearbeiten);
    menu.addAction(ActionPersonLoeschen);
    menu.exec(ui->tablePersonen->viewport()->mapToGlobal(Pos));
}

/*!
 * \brief MainWindow::PersonenAnzeigen füllt die Listview mit Daten der Pesonen
 * \param JFFilter Ausgewählte Zeile der Combobox
 * \param NamenFilter Eingegebener Suchbegriff
 */
void WidgetPersonenTab::PersonenAnzeigen(int JFFilter,QString NamenFilter)
{
    Personen.clear();
    QStringList Zeile;
    Zeile.append("ID");
    Zeile.append("Vorname");
    Zeile.append("Nachname");
    Zeile.append("Jugendfeuerwehr");
    Personen.setHorizontalHeaderLabels(Zeile);
    int FilterNr=0,FilterAns=0;
    if (JFFilter>0)
    {
        FilterAns=1;
        FilterNr=ui->comboBoxPerJFFilter->itemData(JFFilter).toInt();
        std::cout<<FilterNr<<std::endl;
    }
    PersonenTabelle *PerDaten=NULL;
    if (NamenFilter.isEmpty())
        PerDaten=Daten->getPersonen(&FilterNr,FilterAns);
    else
        PerDaten=Daten->getPersonen(&FilterNr,FilterAns,NamenFilter);
    for (int i=0;i<PerDaten->Anzahl;++i)
    {
        QList<QStandardItem*> Zeile;
        Zeile.append(new QStandardItem(QString::number(PerDaten->ID[i])));
        Zeile.append(new QStandardItem(PerDaten->Vorname[i]));
        Zeile.append(new QStandardItem(PerDaten->Nachname[i]));
        Zeile.append(new QStandardItem(PerDaten->JugendFeuerwehr[i]));
        Personen.appendRow(Zeile);
    }
    delete PerDaten;
}

void WidgetPersonenTab::PersonAusgewaehlt(const QModelIndex &neu, const QModelIndex )
{

    QModelIndex Index=ProPersonen.mapToSource(ProPersonen.index(neu.row(),0));
    int row=Index.row();
    int ID=Personen.data(Personen.index(row,0)).toInt();
    PersonGewaehlt(ID);
    return;
}

void WidgetPersonenTab::PersonHinClicked()
{
    QString Vorname= ui->linePerVor->text();
    QString Nachname=ui->lineEditPerNach->text();
    int JF=ui->comboPerJFEin->itemData(ui->comboPerJFEin->currentIndex()).toInt();
    if (Vorname.isEmpty())
    {
        QMessageBox::warning(this,"Vorname Fehlet!",QString::fromUtf8("Bitte Geben sie den Vornamen der Person ein, die hinzugefügt werden soll"));
        return;
    }
    if (Nachname.isEmpty())
    {
        QMessageBox::warning(this,"Nachname Fehlet!",QString::fromUtf8("Bitte Geben sie den Nachnamen der Person ein, die hinzugefügt werden soll"));
        return;
    }
    Daten->addPerson(Nachname,Vorname,JF);
    PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex(),ui->lineEditSuchName->text());
    PersonHinCancel();
}

/*!
 * \brief MainWindow::PersonBearbeitenClicked öffent ein Personbearbeiten Fester für die Ausgewälte Person
 */
void WidgetPersonenTab::PersonBearbeitenClicked()
{
    QModelIndex Index=ProPersonen.mapToSource(ProPersonen.index(ui->tablePersonen->currentIndex().row(),0));
    int ID=Personen.data(Index).toInt();
    PersonBeabeiten->bearbeiten(ID);
    PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex(),ui->lineEditSuchName->text());
}

void WidgetPersonenTab::PersonHinCancel()
{
    ui->lineEditPerNach->clear();
    ui->linePerVor->clear();
}

/*!
 * \brief MainWindow::PersonLoeschen Für den Löschvorgang der markierten Person aus. Und Prüft, ob sie noch Kleidungsstücke ausgelienen hat.
 */
void WidgetPersonenTab::PersonLoeschen()
{
    //Holen des Orginal Indexes.
    QModelIndex Index=ProPersonen.mapToSource(ProPersonen.index(ui->tablePersonen->currentIndex().row(),0));
    int id=Personen.data(Index).toInt();
    KleiderTabelle *Kleider=Daten->getKleidervonPerson(id,0);
    if (Kleider->Anzahl==0)
    {
        if (QMessageBox::information(this,QString::fromUtf8("Person löschen"),QString::fromUtf8("Sind Sie sicher, dass sie ausgewälte die Person löschen wollen?"),
                                      QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        Daten->removePerson(id);
        PersonenAnzeigen(ui->comboBoxPerJFFilter->currentIndex(),ui->lineEditSuchName->text());
    }
    else
    {
        QMessageBox::warning(this,QString::fromUtf8("Die Person hat noch Kleidungsstücke."),QString::fromUtf8("Die Person kann nicht gelöscht werden, da sie noch Kleidungsstücke ausgeliehen hat."),QMessageBox::Ok);
    }
    delete Kleider;
}

/*!
 * \brief De Slot MainWindow::PersonListeDoubleClicked Wächselt die aktive Ansicht des Fenstes.
 * \param Index
 */
void WidgetPersonenTab::PersonListeDoubleClicked(const QModelIndex &)
{   showPerson(PersonenID);
    //ui->tabWidget->setCurrentIndex(2);
}
