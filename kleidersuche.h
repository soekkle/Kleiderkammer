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

#ifndef KLEIDERSUCHE_H
#define KLEIDERSUCHE_H

#include <QDialog>
#include <QMessageBox>
#include "datenquelle.h"

namespace Ui {
class KleiderSuche;
}
//!Klasse für die Oberfläche der Suche nach informationen zu einem Kleidungsstück
/*!
 * \brief Die Klasse KleiderSuche stelt ein Fesnter zum Anzeigen der Infomationen zu einen Kleidungsstück bereit.
 */
class KleiderSuche : public QDialog
{
    Q_OBJECT

public:
    explicit KleiderSuche(DatenQuelle *Daten,QWidget *parent = 0);
    ~KleiderSuche();

private:
    Ui::KleiderSuche *ui;
    DatenQuelle *Daten;

signals:
    void PersonGewaehlt(int ID);

private slots:
    //! Slot zum ausführen der Suche
    void slotSuchen();
    //! Slot zum Ausführen des geklikten Links
    void slotLinkPerson(QString ID);
};

#endif // KLEIDERSUCHE_H
