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
#ifndef PERSONBEARBEITENDIALOG_H
#define PERSONBEARBEITENDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QMessageBox>
#include "DatenQuellen/datenquelle.h"

namespace Ui {
class PersonBearbeitenDialog;
}

class PersonBearbeitenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PersonBearbeitenDialog(DatenQuelle *Daten, QWidget *parent = 0);
    ~PersonBearbeitenDialog();
    bool bearbeiten(int ID);

private:
    Ui::PersonBearbeitenDialog *ui;
    DatenQuelle *Daten;
    int ID,GruppenID;
    QString VName,NName;
    bool DatenGeaendert;

private slots:
    void Buttons(QAbstractButton *button);
    void DatenZuruecksetzen();
    void  accept();
    void DatenAenderung();
};

#endif // PERSONBEARBEITENDIALOG_H
