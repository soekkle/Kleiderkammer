/*
 * Copyright (C) 2016,2017 Sören Krecker
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
