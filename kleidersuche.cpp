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
/*!
 * \brief KleiderSuche::slotSuchen Fürt die suche nach einen Kleidungsstück mit der eingebenen Nummer aus. Und füllt
 *  das Formular mit den Daten.
 */
void KleiderSuche::slotSuchen()
{
    int Nummer=ui->lineEdit->text().toInt();
    int Leihen;
    QString Typ,Name,Gruppe,Groesse,Bemerkung;
    QDate Anschaffung;
    // Prüft ob das Kleidungsstück vorhanden ist.
    if (Daten->getKleidungsInfoByNummer(Nummer,&Typ,&Groesse,&Anschaffung,&Name,&Gruppe,&Bemerkung,&Leihen)>0)
    {
        // Setzen der Felder im Formular
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
    else
    {
        // Leeren der Felder im Formular
        ui->label_Typ->setText("");
        ui->label_Goesse->setText("");
        ui->label_Alte->setText("");
        ui->label_Bemerkung->setText("");
        ui->label_Ort->setText("");
        // Ausgabe einer Infomeldung.
        QMessageBox::information(this,QString::fromUtf8("Kleidungsstück nicht Vohanden."),
                                 QString::fromUtf8("Es gibt kein Kleidungsstück mit der Nummer %1 in der Datenkank.").arg(Nummer));
    }
}
