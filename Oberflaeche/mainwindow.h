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
#include "MainTabs/widgeteinkleidentab.h"
#include "MainTabs/widgetkleidungtab.h"
#include "MainTabs/widgetpersonentab.h"

#include "kleidersuche.h"
#include "kleidungstypenverwaltung.h"
#include "gruppenverwaltung.h"
#include "versioninfo.h"

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
    WidgetEinkleidenTab *EinkleidenTab;
    WidgetKleidungTab *KleidungTab;
    WidgetPersonenTab *PersonenTab;


    SQLiteQuelle *Daten;
    KleiderSuche *KleiderInfoSuchen;
    KleidungsTypenVerwaltung *Typen;
    Gruppenverwaltung *Gruppen;
    QString Ort;

private slots:
    //! Slot der die Komboboxen füllt.
    void ComboboxFuellen();
    //! Zeigt das Infofenser zu diesem Programm
    void ZeigeInfo();
    //! Zeigt den einkleiden Tab zu einer Person
    void ZeigePersonKleider(int ID);
    //! Zeigt das Infofenster zu Qt
    void ZeigeQTInfo();
};

#endif // MAINWINDOW_H
