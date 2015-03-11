#ifndef GRUPPENVERWALTUNG_H
#define GRUPPENVERWALTUNG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QMenu>
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
    //! Actions für das Contextmenü.
    QAction *ActionGruppeLoeschen;

    void gruppenTabelleFullen();

private slots:
    void gruppenHin();
    void GruppeLoeschen();
    void GruppenContextMenu(QPoint Pos);
};

#endif // GRUPPENVERWALTUNG_H
