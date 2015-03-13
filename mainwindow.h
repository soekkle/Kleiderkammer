#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPrintDialog>
#include "datenquelle.h"
#include "sqlitequelle.h"
#include "kleidungstypenverwaltung.h"
#include "gruppenverwaltung.h"
#include "bericht.h"
#include "kleidungstableview.h"

#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#if __WIN32__||_MSC_VER
#define WINVER 0x0602
#define _WIN32_WINNT 0x0602
#include <windows.h>
#include <shlobj.h>
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Bericht *Drucken;
    SQLiteQuelle *Daten;
    KleidungsTypenVerwaltung *Typen;
    Gruppenverwaltung *Gruppen;
    QString Ort;
    //! Modelle für das Anzeigen der Daten.
    QStandardItemModel Personen;
    KleidungsTableview *Kleidungstuecke, *PerKleider, *KleiderAus;
    //! Modelle Für die Sortierten Anzeigen.
    QSortFilterProxyModel ProPersonen, ProKleidungstuecke, ProPerKleider, ProKleiderAus;
    int PersonenID;
    //! Die verschieden Actionen.
    QAction *ActionPersonLoeschen,*ActionKleicungLoeschen;

private slots:
    void AusGroessenFiltergeaendert(int Typ);
    void Auslehenclicked();
    void AusTypFiltergeaendert(int Typ);
    void BerichtAnzeigen();
    //! Slot zum Starten des Druckens
    void BerichtDrucken();
    //! Slot zum Speichern des Berichtes als HTML-Datei.
    void BerichtSpeichern();
    void ComboboxFuellen();
    //! Solt zum Anzeigen des Contextmenüs in der Kleidungs Tabelle
    void KleidungContextMenuEvent(const QPoint Pos);
    //! Slot zum Löschen eines Kleidungsstückes.
    void KleidungLoeschen();
    //! Slot zum Anpassen der Maske zum Anlegen neuer Kleidungstücke.
    void Kleidungstypgewaehlt(int Typ);
    //! Slot der für die Anzeige der Personen zuständig ist.
    void PersonenAnzeigen(int Filter);
    //! Slot der für die Anzeige der Kleiderstücke zusändig ist.
    void KleidunginKammerAnzeigen(int Filter);
    void KleidungHinClicked();
    void KleidungHinCancel();
    //! Slot für das ContextMenü bei den Namen
    void NamenContextMenuEvent(const QPoint &Pos);
    void PerKleidungslistefuellen(int FilterTyp);
    //! Slot der die Eingabemaske für Personen zurücksetzt.
    void PersonHinCancel();
    //! Solt der die Eingegebene Person Speichert.
    void PersonHinClicked();
    //! Füllt den Tab Für einen Person.
    void PersonAusgewaehlt(const QModelIndex &neu,const QModelIndex);
    //! Löscht die Ausgewälte Person.
    void PersonLoeschen();
    void Zurueckgeben();

};

#endif // MAINWINDOW_H
