#ifndef KLEIDUNGSTYPENVERWALTUNG_H
#define KLEIDUNGSTYPENVERWALTUNG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <datenquelle.h>

namespace Ui {
class KleidungsTypenVerwaltung;
}

class KleidungsTypenVerwaltung : public QDialog
{
    Q_OBJECT
    
public:
    explicit KleidungsTypenVerwaltung(DatenQuelle *Quelle,QWidget *parent = 0);
    ~KleidungsTypenVerwaltung();
    
private:

    void Typentable();
    void GroessenList();

    Ui::KleidungsTypenVerwaltung *ui;
    DatenQuelle *Daten;
    QStandardItemModel *Typen,*Groessen;
    int GroessenFilter;

signals:
    void datenGeaendert();

public slots:
        int exec();
private slots:
        void GroesseAnlegen();
        void Typanlegen();
        void Typwahlen(const QItemSelection& neu  , const QItemSelection &);
};

#endif // KLEIDUNGSTYPENVERWALTUNG_H
