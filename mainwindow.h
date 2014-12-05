#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datenquelle.h"
#include "sqlitequelle.h"
#include "kleidungstypenverwaltung.h"

#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    SQLiteQuelle *Daten;
    KleidungsTypenVerwaltung *Typen;

    QStandardItemModel Personen,Kleidungstuecke;

private slots:

    void PersonenAnzeigen(int Filter);
    void PersonHinCancel();
    void PersonHinClicked();

};

#endif // MAINWINDOW_H
