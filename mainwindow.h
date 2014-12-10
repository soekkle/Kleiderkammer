#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datenquelle.h"
#include "sqlitequelle.h"
#include "kleidungstypenverwaltung.h"

#include <QStandardItemModel>

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
    SQLiteQuelle *Daten;
    KleidungsTypenVerwaltung *Typen;
    //! Modelle für das Anzeigen der Daten.
    QStandardItemModel Personen,Kleidungstuecke;
    void ComboboxFuellen();

private slots:
    //! Slot zum Anpassen der Maske zum Anlegen neuer Kleidungstücke.
    void Kleidungstypgewaehlt(int Typ);
    //! Slot der für die Anzeige der Personen zuständig ist.
    void PersonenAnzeigen(int Filter);
    //! Slot der für die Anzeige der Kleiderstücke zusändig ist.
    void KleidunginKammerAnzeigen(int Filter);
    void KleidungHinClicked();
    void KleidungHinCancel();
    //! Slot der die Eingabemaske für Personen zurücksetzt.
    void PersonHinCancel();
    //! Solt der die Eingegebene Person Speichert.
    void PersonHinClicked();

};

#endif // MAINWINDOW_H
