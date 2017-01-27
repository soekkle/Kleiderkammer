#ifndef WIDGETEINKLEIDENTAB_H
#define WIDGETEINKLEIDENTAB_H

#include <QFrame>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QMenu>

#include "DatenQuellen/datenquelle.h"
#include "tableviews/kleidungstableview.h"
#include "tableviews/itemsdelegate.h"

namespace Ui {
class WidgetEinkleidenTab;
}

class WidgetEinkleidenTab : public QFrame
{
    Q_OBJECT

public:
    explicit WidgetEinkleidenTab(DatenQuelle *Daten, QWidget *parent = 0);
    ~WidgetEinkleidenTab();
    //! Funktion zum Aufbereiten der Ausleihen Maske.
public slots:
    void DatenGeaendert();
    void showPerson(int ID);
signals:
    void KleidungVerschoben();
private:
    Ui::WidgetEinkleidenTab *ui;
    DatenQuelle *Daten;
    ComboboxGroessenDelegate *ComboBox;

    KleidungsTableview *PerKleider, *KleiderAus;
    QSortFilterProxyModel ProPerKleider, ProKleiderAus;
    int PersonenID;

    QAction *ActKleiderRuck;
    QAction *ActKleiderAus;
    QAction *ActLoeschenKleidungPerson;
    QAction *ActLoeschenKleidungKammer;

private slots:
    void AusGroessenFiltergeaendert(int Typ);
    void Auslehenclicked();
    void AusTypFiltergeaendert(int Typ);
    //! Slot der Aufgerufen wird, wenn der LineEdit_AusNum geändert wurde
    void LineEditAusNummerChange(QString Nummer);
    void PerKleidungslistefuellen(int FilterTyp);

    void loeschenKleidungPerson();
    void loeschenKleidungKammer();

    void TablePerKleiderContextMenuEvent(const QPoint Pos);
    void TableKammerKJleiderContextMenuEvent(const QPoint Pos);
    void Zurueckgeben();
};

#endif // WIDGETEINKLEIDENTAB_H
