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
