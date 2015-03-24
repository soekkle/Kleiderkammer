#ifndef KLEIDUNGSTYPENVERWALTUNG_H
#define KLEIDUNGSTYPENVERWALTUNG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QMenu>
#include <QMessageBox>
#include <datenquelle.h>
#include "tableviews/spinboxdelegate.h"
#include "tableviews/kleidungsgroessentableview.h"

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

    Ui::KleidungsTypenVerwaltung *ui;
    DatenQuelle *Daten;
    QStandardItemModel *Typen;
    KleidungsGroessenTableView *Groessen;
    int GroessenFilter;
    SpinBoxDelegate SpinBox;
    QAction *ActionTypLoeschen,*ActionGroesseLoeschen;

signals:
    void datenGeaendert();

public slots:
        int exec();
private slots:
        void GroesseAnlegen();
        //! Zeigt das Contextmenue Für die Tableview mit den Größen an
        void GrossenContextMenu(QPoint Pos);
        //! Löscht die Aktuell ausgewälte Größe.
        void GroesseLoeschen();
        void Typanlegen();
        //! Zeigt das Contextmenü in der Tablevie mit den Typen.
        void TypContextMenu(QPoint Pos);
        //! Löscht die Aktuell Ausgewälte Größe.
        void TypLoeschen();
        void Typwahlen(const QItemSelection& neu  , const QItemSelection &);
};
#endif // KLEIDUNGSTYPENVERWALTUNG_H
