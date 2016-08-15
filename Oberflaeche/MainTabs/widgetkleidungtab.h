#ifndef WIDGETKLEIDUNGTAB_H
#define WIDGETKLEIDUNGTAB_H

#include <QFrame>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QMenu>

#include "DatenQuellen/datenquelle.h"
#include "tableviews/kleidungstableview.h"
#include "tableviews/itemsdelegate.h"

namespace Ui {
class WidgetKleidungTab;
}

//! Diese Klasse stellt die Funktionen für den Tab Kleidungsstücke bereit.
/*!
 * \brief Die Klasse WidgetBerichtTab stellt die Funktionen Des Tabs Kleidungsstücke im Hauptfenster zur verfügung.
 * Auch die Lokik zu Anlegen, Löschen usw. für den Tab enthalten.
 */
class WidgetKleidungTab : public QFrame
{
    Q_OBJECT

public:
    explicit WidgetKleidungTab(DatenQuelle *Daten,QWidget *parent = 0);
    ~WidgetKleidungTab();
public slots:
    void DatenGeaendert();
    void refrashTable();
private:
    Ui::WidgetKleidungTab *ui;

    QAction *ActionKleicungLoeschen;
    ComboboxGroessenDelegate *ComboBox;
    DatenQuelle *Daten;
    KleidungsTableview *Kleidungstuecke;
    QSortFilterProxyModel ProKleidungstuecke;

private slots:
    void HinCancel();
    void HinClicked();
    //! Slot der für die Anzeige der Kleiderstücke zusändig ist.
    void KleidunginKammerAnzeigen(int Filter);
    //! Solt zum Anzeigen des Contextmenüs in der Kleidungs Tabelle
    void TableContextMenuEvent(const QPoint Pos);
    //! Slot zum Anpassen der Maske zum Anlegen neuer Kleidungstücke.
    void Kleidungstypgewaehlt(int Typ);
    //! Slot zum Löschen eines Kleidungsstückes.
    void KleidungLoeschen();
};

#endif // WIDGETKLEIDUNGTAB_H
