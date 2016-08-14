#ifndef WIDGETEINKELEIDENTAB_H
#define WIDGETEINKELEIDENTAB_H

#include <QFrame>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include "DatenQuellen/datenquelle.h"
#include "tableviews/kleidungstableview.h"
#include "tableviews/itemsdelegate.h"

namespace Ui {
class WidgetEinkeleidenTab;
}

class WidgetEinkeleidenTab : public QFrame
{
    Q_OBJECT

public:
    explicit WidgetEinkeleidenTab(DatenQuelle *Daten, QWidget *parent = 0);
    ~WidgetEinkeleidenTab();
    //! Funktion zum Aufbereiten der Ausleihen Maske.
    void showPerson(int ID);
public slots:
    void DatenGeaendert();
signals:
    void KleidungVerschoben();
private:
    Ui::WidgetEinkeleidenTab *ui;
    DatenQuelle *Daten;
    ComboboxGroessenDelegate *ComboBox;

    KleidungsTableview *PerKleider, *KleiderAus;
    QSortFilterProxyModel ProPerKleider, ProKleiderAus;
    int PersonenID;

private slots:
    void AusGroessenFiltergeaendert(int Typ);
    void Auslehenclicked();
    void AusTypFiltergeaendert(int Typ);
    //! Slot der Aufgerufen wird, wenn der LineEdit_AusNum geändert wurde
    void LineEditAusNummerChange(QString Nummer);
    void PerKleidungslistefuellen(int FilterTyp);
    void Zurueckgeben();
};

#endif // WIDGETEINKELEIDENTAB_H
