/*
 * Copyright (C) 2014,2015 Sören Krecker
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
#include <QFileDialog>
#include <QPrintDialog>
#include "datenquelle.h"
#include "sqlitequelle.h"
#include "kleidersuche.h"
#include "kleidungstypenverwaltung.h"
#include "gruppenverwaltung.h"
#include "bericht.h"
#include "tableviews/kleidungstableview.h"
#include "tableviews/itemsdelegate.h"
#include "versioninfo.h"

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTextStream>
#include <QCheckBox>

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

    ComboboxGroessenDelegate *ComboBox;
    QVector<QCheckBox*> CheckBoxBeType;
    Bericht *Drucken;
    SQLiteQuelle *Daten;
    KleiderSuche *KleiderInfoSuchen;
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
    //! Funktion zum Aufbereiten der Ausleihen Maske.
    void PersonAusleih(int ID);
    //! Funktion für die Anzeige der Personen mit den Übergebenen Parametern.
    void PersonenAnzeigen(int JFFilter, QString NamenFilter);

private slots:
    void AusGroessenFiltergeaendert(int Typ);
    void Auslehenclicked();
    void AusTypFiltergeaendert(int Typ);
    //! Slot zum Anzagen des Beichtes
    void BerichtAnzeigen();
    //! Slot zum Starten des Druckens
    void BerichtDrucken();
    //! Slot zum Speichern des Berichtes als HTML-Datei.
    void BerichtSpeichern();
    //! Slot der die Komboboxen füllt.
    void ComboboxFuellen();
    //! Slot, der nach dem Ändern der Combobox ComboboxPerJFFilter geändert wurde.
    void ComboboxPerJFFilterGewahlt(int Pos);
    //! Slot der die Checkboxen im Bericht ausblendet.
    void Groupchecked(bool checked);
    //! Solt zum Anzeigen des Contextmenüs in der Kleidungs Tabelle
    void KleidungContextMenuEvent(const QPoint Pos);
    //! Slot zum Löschen eines Kleidungsstückes.
    void KleidungLoeschen();
    //! Slot zum Anpassen der Maske zum Anlegen neuer Kleidungstücke.
    void Kleidungstypgewaehlt(int Typ);
    //! Slot der für die Anzeige der Kleiderstücke zusändig ist.
    void KleidunginKammerAnzeigen(int Filter);
    void KleidungHinClicked();
    void KleidungHinCancel();
    //! Slot der Aufgerufen wird wenn der LineEditSuchName geändert wurde
    void LineEditSuchNameChange(QString SuchFilter);
    //! Slot für das ContextMenü bei den Namen
    void NamenContextMenuEvent(const QPoint &Pos);
    void PerKleidungslistefuellen(int FilterTyp);
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
    //! Aktivirt und deaktivirt die Combobox
    void RadiobuttomCilcked();
    //! Zeigt das Infofenser zu diesem Programm
    void ZeigeInfo();
    //! Zeigt den einkleiden Tab zu einer Person
    void ZeigePersonKleider(int ID);
    //! Zeigt das Infofenster zu Qt
    void ZeigeQTInfo();
    void Zurueckgeben();

};

#endif // MAINWINDOW_H
