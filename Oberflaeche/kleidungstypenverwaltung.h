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

#ifndef KLEIDUNGSTYPENVERWALTUNG_H
#define KLEIDUNGSTYPENVERWALTUNG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QMenu>
#include <QMessageBox>
#include <DatenQuellen/datenquelle.h>
#include "tableviews/itemsdelegate.h"
#include "tableviews/kleidungsgroessentableview.h"

namespace Ui {
class KleidungsTypenVerwaltung;
}
//! Diese Klasse stellt einen Dialog zum bearbeiten von den Kleidungstypen und den dazugehörigen Größen bereit.
class KleidungsTypenVerwaltung : public QDialog
{
    Q_OBJECT
    
public:
    explicit KleidungsTypenVerwaltung(DatenQuelle *Quelle,QWidget *parent = 0);
    ~KleidungsTypenVerwaltung();
    
private:

    void Typentable();

    Ui::KleidungsTypenVerwaltung *ui;
    DatenQuelle *Daten;
    QStandardItemModel *Typen;
    KleidungsGroessenTableView *Groessen;
    int GroessenFilter;
    SpinBoxDelegate SpinBox;
    QAction *ActionTypLoeschen,*ActionGroesseLoeschen;

signals:
    void datenGeaendert();

public slots:
        //! Ausführen des Dialoges
        int exec();
private slots:
        //! Anlegen der angegbenen Größe
        void GroesseAnlegen();
        //! Zeigt das Contextmenue Für die Tableview mit den Größen an
        void GrossenContextMenu(QPoint Pos);
        //! Löscht die Aktuell ausgewälte Größe.
        void GroesseLoeschen();
        void Typanlegen();
        //! Zeigt das Contextmenü in der Tablevie mit den Typen.
        void TypContextMenu(QPoint Pos);
        //! Löscht die Aktuell Ausgewälte Größe.
        void TypLoeschen();
        void Typwahlen(const QItemSelection& neu  , const QItemSelection &);
};
#endif // KLEIDUNGSTYPENVERWALTUNG_H
