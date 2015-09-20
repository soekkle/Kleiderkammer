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

private slots:
    //! Slot zum ausführen der Suche
    void slotSuchen();
};

#endif // KLEIDERSUCHE_H
