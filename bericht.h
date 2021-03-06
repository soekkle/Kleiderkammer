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

#ifndef BERICHT_H
#define BERICHT_H

#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include "DatenQuellen/datenquelle.h"



class Bericht
{
public:
    Bericht(DatenQuelle *Daten, QString Ort);
    ~Bericht();
    //! gibt an ob die CSS datei extern bleibt oder eingebunden wird.
    bool CSSextern;
    //! Gibt den HTML-Code für die Personenliste für die Angegebene Gruppe zurück
    QString generierenPersonenListe(int Gruppe, QVector<int> TypenListe=QVector<int>());
    //! Gibt den HTML-Code für die Inventurliste zurück.
    QString generiereKammerListe();
    //! Setzt den Pfard auf die zu verwendene CSS-Datei
    bool setCSS(QString Ort);


private:
    DatenQuelle *Daten;
    QString CSSFile;
    QString FileDir;
    //! Generirt CSS Bereich im Header und gibt ihn zurück.
    QString CSSHeader();
};

#endif // BERICHT_H
