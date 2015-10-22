/*
 * Copyright (C) 2015 Sören Krecker
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
 * Diese Datei ist Teil von kleiderkammer.
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

#ifndef KLEIDUNGSGROESSENTABLEVIEW_H
#define KLEIDUNGSGROESSENTABLEVIEW_H

#include <QAbstractTableModel>
#include <DatenQuellen/datenquelle.h>
/*!
 * \brief Die KleidungsGroessenTableView Klasse bereitet die Daten zum anzeigenin der Größen einer Tablewiev auf.
 * Und bietet die möglichkeit den Rang nach dem die Größen sortiert werden anzupassen.
 * \author Sören Krecker
 * \version 0.1
 */
class KleidungsGroessenTableView : public QAbstractTableModel
{
public:
    KleidungsGroessenTableView(DatenQuelle* Daten, QObject *parent = 0);
    int columnCount(const QModelIndex &) const;
    void clear();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int getGroessenID(int row);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    void setKleidungsTyp(int Typ);

public slots:
    void update();

private:
    //! Zeiger Auf die Datenquelle aus der Die Daten Entnommen Werden.
    DatenQuelle *Daten;
    //! Zeiger Auf die Zwieschengespeicherten ergebnisse die Angezeigt werden.
    GroessenTabelle *Groessen;
    //! Speicher für den Kleidungstyp für den die Größe angezeigt werden soll.
    int Typ;
};

#endif // KLEIDUNGSGROESSENTABLEVIEW_H
