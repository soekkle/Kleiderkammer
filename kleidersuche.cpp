#include "kleidersuche.h"
#include "ui_kleidersuche.h"

KleiderSuche::KleiderSuche(DatenQuelle *Daten, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KleiderSuche)
{
    ui->setupUi(this);
    this->Daten=Daten;
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slotSuchen()));
}

KleiderSuche::~KleiderSuche()
{
    delete ui;
}

void KleiderSuche::slotSuchen()
{
    int Nummer=ui->lineEdit->text().toInt();
    int Leihen;
    QString Typ,Name,Gruppe,Groesse,Bemerkung;
    QDate Anschaffung;
    if (Daten->getKleidungsInfoByNummer(Nummer,&Typ,&Groesse,&Anschaffung,&Name,&Gruppe,&Bemerkung,&Leihen)>0)
    {
        ui->label_Typ->setText(Typ);
        ui->label_Goesse->setText(Groesse);
        ui->label_Alte->setText(Anschaffung.toString("dd.MM.yyyy"));
        ui->label_Bemerkung->setText(Bemerkung);
        if (Name.isEmpty())
        {
            ui->label_Ort->setText(QString::fromUtf8("Kleidungsstück ist in der Kleiderkammer."));
        }
        else {
            Name.append(" - ").append(Gruppe);
            ui->label_Ort->setText(Name);
        }
    }
}
