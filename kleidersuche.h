#ifndef KLEIDERSUCHE_H
#define KLEIDERSUCHE_H

#include <QDialog>
#include <QMessageBox>
#include "datenquelle.h"

namespace Ui {
class KleiderSuche;
}

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
    void slotSuchen();
};

#endif // KLEIDERSUCHE_H
