#ifndef GRUPPENVERWALTUNG_H
#define GRUPPENVERWALTUNG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include "datenquelle.h"

namespace Ui {
class Gruppenverwaltung;
}

class Gruppenverwaltung : public QDialog
{
    Q_OBJECT
    
public:
    explicit Gruppenverwaltung(DatenQuelle *Daten,QWidget *parent = 0);
    ~Gruppenverwaltung();
    
signals:
    void datenGeaendert();

private:
    Ui::Gruppenverwaltung *ui;
    DatenQuelle *Daten;
    QStandardItemModel Gruppen;

    void gruppenTabelleFullen();
private slots:
    void gruppenHin();
};

#endif // GRUPPENVERWALTUNG_H
