/*
 * Copyright (C) 2016 Sören Krecker
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

#ifndef WIDGETBERICHTTAB_H
#define WIDGETBERICHTTAB_H

#include <QFrame>
#include <QCheckBox>
#include <QFileDialog>
#ifndef NOPRINT
#include <QPrintDialog>
#endif

#include "DatenQuellen/datenquelle.h"
#include "bericht.h"


namespace Ui {
class WidgetBerichtTab;
}

class WidgetBerichtTab : public QFrame
{
    Q_OBJECT

public:
    explicit WidgetBerichtTab(DatenQuelle *Daten, QString Ort, QWidget *parent = 0);
    ~WidgetBerichtTab();

public slots:
    void DatenGeaendert();

private:
    Ui::WidgetBerichtTab *ui;
    QVector<QCheckBox*> CheckBoxBeType;
    Bericht *Drucken;
    DatenQuelle *Daten;
    QString Ort;

    //! Die Fuktion liefert die Ausgwählten Spalten für den Bericht
    QVector<int> BerichtSpalten();

private slots:
    //! Slot zum Anzagen des Beichtes
    void BerichtAnzeigen();
#ifndef NOPRINT
    //! Slot zum Starten des Druckens
    void BerichtDrucken();
#endif
    //! Slot zum Speichern des Berichtes als HTML-Datei.
    void BerichtSpeichern();
    //! Slot der die Checkboxen im Bericht ausblendet.
    void Groupchecked(bool checked);
    //! Aktivirt und deaktivirt die Combobox
    void RadiobuttomCilcked();
};

#endif // WIDGETBERICHTTAB_H
