#ifndef EINSTELLUNGEN_H
#define EINSTELLUNGEN_H

#include <QDialog>

#include "EinstellungenTabs/widgeteincsstab.h"

namespace Ui {
class Einstellungen;
}

class Einstellungen : public QDialog
{
    Q_OBJECT

public:
    explicit Einstellungen(QWidget *parent = 0);
    ~Einstellungen();

private:
    Ui::Einstellungen *ui;
    WidgetEinCSSTab *CSSTab;
};

#endif // EINSTELLUNGEN_H
