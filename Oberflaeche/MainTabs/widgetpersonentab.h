#ifndef WIDGETPERSONENTAB_H
#define WIDGETPERSONENTAB_H

#include <QFrame>
#include <QMenu>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include <iostream>

#include "DatenQuellen/datenquelle.h"
#include "Oberflaeche/personbearbeitendialog.h"



namespace Ui {
class WidgetPersonenTab;
}

class WidgetPersonenTab : public QFrame
{
    Q_OBJECT

public:
    explicit WidgetPersonenTab(DatenQuelle *Daten,QWidget *parent = 0);
    ~WidgetPersonenTab();
public slots:
    void DatenGeaendert();
signals:
    void PersonGewaehlt(int PID);
    void showPerson(int PID);

private:
    Ui::WidgetPersonenTab *ui;
    DatenQuelle *Daten;
    //! Modelle für das Anzeigen der Daten.
    QStandardItemModel Personen;
    //! Modelle Für die Sortierten Anzeigen.
    QSortFilterProxyModel ProPersonen;
    int PersonenID;
    //! Die verschieden Actionen.
    QAction *ActionPersonLoeschen,*ActionPersonBearbeiten;
    //* Pointer auf den BersonenBearbeiten Dialog
    PersonBearbeitenDialog *PersonBeabeiten;

    //! Funktion für die Anzeige der Personen mit den Übergebenen Parametern.
    void PersonenAnzeigen(int JFFilter, QString NamenFilter);
private slots:
    //! Slot, der nach dem Ändern der Combobox ComboboxPerJFFilter geändert wurde.
    void ComboboxPerJFFilterGewahlt(int Pos);
    //! Slot der Aufgerufen wird, wenn der LineEditSuchName geändert wurde
    void LineEditSuchNameChange(QString SuchFilter);
    //! Slot für das ContextMenü bei den Namen
    void NamenContextMenuEvent(const QPoint &Pos);
    //! Slot Zum Bearbeiten einer Person.
    void PersonBearbeitenClicked();
    //! Slot der die Eingabemaske für Personen zurücksetzt.
    void PersonHinCancel();
    //! Solt der die Eingegebene Person Speichert.
    void PersonHinClicked();
    //! Füllt den Tab Für einen Person.
    void PersonAusgewaehlt(const QModelIndex &neu,const QModelIndex);
    //! Wächselt die Ansuicht
    void PersonListeDoubleClicked(const QModelIndex &Index);
    //! Löscht die Ausgewälte Person.
    void PersonLoeschen();
};

#endif // WIDGETPERSONENTAB_H
