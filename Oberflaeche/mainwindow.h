/*
 * Copyright (C) 2014-2016 Sören Krecker
 *
 * This file is part of Kleiderkammer.
 *
 * Kleiderkammer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation version 3 of the License.
 *
 * Kleiderkammer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Kleiderkammer.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Diese Datei ist Teil von Kleiderkammer.
 *
 * Kleiderkammer ist Freie Software: Sie können es unter den Bedingungen
 * der GNU Affero General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz, weiterverbreiten und/oder modifizieren.
 *
 * Kleiderkammer wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN
 * ZWECK.Siehe die GNU Affero General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU Affero General Public License zusammen mit
 * diesem Programm erhalten haben. Wenn nicht, siehe
 * <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "DatenQuellen/datenquelle.h"
#include "DatenQuellen/sqlitequelle.h"
// Einbinden der TabWidgets
#include "MainTabs/widgetberichttab.h"
#include "MainTabs/widgetkleidungtab.h"

#include "kleidersuche.h"
#include "kleidungstypenverwaltung.h"
#include "gruppenverwaltung.h"
#include "personbearbeitendialog.h"
#include "tableviews/kleidungstableview.h"
#include "tableviews/itemsdelegate.h"
#include "versioninfo.h"

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTextStream>

#if __WIN32__||_MSC_VER
//#define WINVER 0x0602
//#define _WIN32_WINNT 0x0602
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
    WidgetBerichtTab *BerichtTab;
    WidgetKleidungTab *KleidungTab;

    ComboboxGroessenDelegate *ComboBox;

    SQLiteQuelle *Daten;
    KleiderSuche *KleiderInfoSuchen;
    KleidungsTypenVerwaltung *Typen;
    Gruppenverwaltung *Gruppen;
    //* Pointer auf den BersonenBearbeiten Dialog
    PersonBearbeitenDialog *PersonBeabeiten;
    QString Ort;
    //! Modelle für das Anzeigen der Daten.
    QStandardItemModel Personen;
    KleidungsTableview *PerKleider, *KleiderAus;
    //! Modelle Für die Sortierten Anzeigen.
    QSortFilterProxyModel ProPersonen, ProPerKleider, ProKleiderAus;
    int PersonenID;
    //! Die verschieden Actionen.
    QAction *ActionPersonLoeschen,*ActionPersonBearbeiten;
    //! Funktion zum Aufbereiten der Ausleihen Maske.
    void PersonAusleih(int ID);
    //! Funktion für die Anzeige der Personen mit den Übergebenen Parametern.
    void PersonenAnzeigen(int JFFilter, QString NamenFilter);

private slots:
    void AusGroessenFiltergeaendert(int Typ);
    void Auslehenclicked();
    void AusTypFiltergeaendert(int Typ);
    //! Slot der die Komboboxen füllt.
    void ComboboxFuellen();
    //! Slot, der nach dem Ändern der Combobox ComboboxPerJFFilter geändert wurde.
    void ComboboxPerJFFilterGewahlt(int Pos);
    //! Slot der Aufgerufen wird, wenn der LineEditSuchName geändert wurde
    void LineEditSuchNameChange(QString SuchFilter);
    //! Slot der Aufgerufen wird, wenn der LineEdit_AusNum geändert wurde
    void LineEditAusNummerChange(QString Nummer);
    //! Slot für das ContextMenü bei den Namen
    void NamenContextMenuEvent(const QPoint &Pos);
    void PerKleidungslistefuellen(int FilterTyp);
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
    //! Zeigt das Infofenser zu diesem Programm
    void ZeigeInfo();
    //! Zeigt den einkleiden Tab zu einer Person
    void ZeigePersonKleider(int ID);
    //! Zeigt das Infofenster zu Qt
    void ZeigeQTInfo();
    void Zurueckgeben();
};

#endif // MAINWINDOW_H
